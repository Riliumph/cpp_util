#include "server.h"
// STL
#include <iostream>
#include <stdlib.h>
#include <string>
// system
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
// original
#include "network/util.h"
namespace nw::ipv4::tcp {
/// @brief コンストラクタ
/// @param e_handler イベントハンドラ
/// @param port サーバーポート番号
/// @param hint IPv4のヒント情報
Server::Server(u_short port, struct addrinfo hint)
  : SocketServer(port, hint)
  , timeout_{ 0, 0 }
{
  std::fill(client_fds_.begin(), client_fds_.end(), DISABLE_FD);
}

/// @brief デストラクタ
Server::~Server()
{
  SafeClose();
}

void
Server::EventHandler(std::shared_ptr<event::IF::EventHandler> eh)
{
  event_handler_ = eh;
}

/// @brief タイムアウト時間を設定する
/// @param tm
void
Server::Timeout(struct timeval tm)
{
  timeout_ = tm;
}

/// @brief タイムアウト時間を設定する
/// @param sec 秒数
/// @param usec マイクロ秒数
void
Server::Timeout(time_t sec, suseconds_t usec)
{
  timeout_.tv_sec = sec;
  timeout_.tv_usec = usec;
}

/// @brief コールバックイベントの設定
/// @param e イベント
void
Server::Event(event::IF::EventHandler::callback e)
{
  event_ = e;
}

/// @brief サーバーを立ち上げる関数
/// @return 成否
int
Server::Establish()
{
  Identify();
  auto ok = CreateSocket();
  if (ok < 0) {
    return ok;
  }
  ok = ReuseAddress(server_fd_);
  if (ok < 0) {
    return ok;
  }
  ok = AttachAddress();
  if (ok < 0) {
    return ok;
  }
  ok = Listen();
  if (ok < 0) {
    return ok;
  }
  return ok;
}

/// @brief サーバー処理をスタートする関数
/// @return 成否
bool
Server::Start()
{
  if (!event_handler_) {
    std::cerr << "event_handler not set" << std::endl;
    return false;
  }
  if (!event_handler_->CanReady()) {
    std::cerr << "event_handler not ready" << std::endl;
    return false;
  }
  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = server_fd_;
  auto ok = event_handler_->CreateTrigger(ev.data.fd, ev.events);
  if (ok != 0) {
    std::cerr << "failed to set event" << std::endl;
    return false;
  }
  event_handler_->SetCallback(
    ev.data.fd, ev.events, [this](int fd) { AcceptEvent(fd); });

  std::cout << "start event_handler" << std::endl;
  event_handler_->Run();
  return true;
}

/// @brief ヒント情報からアドレス情報を決定する
/// @param hint 不完全なaddrinfo構造体
/// @param service_name サービス名
/// Linux: /etc/services
/// Windows: C:\Windows\system32\drivers\etc\services
/// @return
int
Server::Identify(std::string service_name)
{
  if (service_name.empty()) {
    service_name = std::to_string(port_);
  }
  // ヒント変数からアドレスを決定し、inet0変数へ設定
  // docに解説有り
  auto err = getaddrinfo(NULL, service_name.data(), hint_, &inet0_);
  if (err != 0) {
    std::cerr << "getaddrinfo(): " << gai_strerror(err) << std::endl;
    return -1;
  }

  err = getnameinfo(inet0_->ai_addr,
                    inet0_->ai_addrlen,
                    host_name_, // output引数
                    sizeof(host_name_),
                    serv_name_, // output引数
                    sizeof(serv_name_),
                    NI_NUMERICHOST | NI_NUMERICSERV);
  if (err != 0) {
    std::cerr << "getnameinfo(): " << gai_strerror(err) << std::endl;
    freeaddrinfo(inet0_);
    return -1;
  }
  printf("identify: %s:%s\n", host_name_, serv_name_);
  return 0;
}

/// @brief サーバーソケットを生成する関数
/// @return 成否
int
Server::CreateSocket()
{
  // addrinfo型はリンクリストを形成するため、forで対応する
  for (auto* info = inet0_; info != nullptr; info = info->ai_next) {
    server_fd_ =
      socket(inet0_->ai_family, inet0_->ai_socktype, inet0_->ai_protocol);
    if (server_fd_ < 0) {
      perror("make socket");
      continue;
    }
    // bindのチェックまでやった方がいい
  }
  std::cout << "server_fd_: " << server_fd_ << std::endl;
  return server_fd_;
}

/// @brief サーバーソケットにアドレスをバインドする
/// @return 成否
int
Server::AttachAddress()
{
  auto ok = bind(server_fd_, inet0_->ai_addr, inet0_->ai_addrlen);
  if (ok < 0) {
    perror("bind");
    return ok;
  }
  return ok;
}

/// @brief サーバーがリッスンする
/// @return 成否
int
Server::Listen() const
{
  auto err = listen(server_fd_, QUEUE_SIZE);
  if (err < 0) {
    perror("listen");
    return err;
  }
  return err;
}

/// @brief 接続に来たクライアントをAcceptする
/// @return クライアントに割り当てられたfd値
int
Server::Accept() const
{
  struct sockaddr_storage from;
  auto len = (socklen_t)sizeof(from);
  auto client_fd = accept(server_fd_, (struct sockaddr*)&from, &len);
  if (client_fd < 0) {
    perror("accept");
    return client_fd;
  }
  char from_host[NI_MAXHOST];
  char from_port[NI_MAXSERV];
  (void)getnameinfo((struct sockaddr*)&from,
                    len,
                    from_host,
                    sizeof(from_host),
                    from_port,
                    sizeof(from_port),
                    NI_NUMERICHOST | NI_NUMERICSERV);
  printf("accept from: %s:%s\n", from_host, from_port);
  return client_fd;
}

/// @brief 今の接続数を取得する
/// @return 接続数
int
Server::CurrentConnection()
{
  auto count = std::count_if(client_fds_.begin(),
                             client_fds_.end(),
                             [](int fd) { return fd != DISABLE_FD; });
  return count;
}

/// @brief 流量制限処理
/// @param client_fd 新しく接続されたクライアントのfd
/// @return 管理番号
int
Server::ControlMaxConnection(const int client_fd)
{
  auto* it = std::find(client_fds_.begin(), client_fds_.end(), DISABLE_FD);
  if (it == client_fds_.end()) {
    return -1;
  }
  *it = client_fd;
  printf("now clients: %d/%ld\n", CurrentConnection(), client_fds_.size());
  auto index = std::distance(client_fds_.begin(), it);
  return index;
}

/// @brief クライアントから切断を受けたときのイベント
/// @param fd 切断にきたクライアントのFD
/// @return 成否
bool
Server::CloseEvent(int fd)
{
  std::cout << "Client disconnected: " << fd << std::endl;
  event_handler_->EraseCallback(fd);
  event_handler_->DeleteTrigger(fd, EPOLLIN);
  close(fd);
  return true;
}

/// @brief クライアントから接続を受けたときのイベント
/// @param server_fd_ サーバーのFD（イベントIF上必要なだけで未使用）
/// @return 成否
bool
Server::AcceptEvent(int server_fd)
{
  (void)server_fd;
  std::cout << "accepting new comer ..." << std::endl;
  auto client_fd = Accept();
  if (client_fd < 0) {
    return false;
  }
  // 最大接続数制限のために独自管理が必要
  auto idx = ControlMaxConnection(client_fd);
  if (idx < 0) {
    std::cerr << "reject new comer for limit connection" << std::endl;
    close(client_fd);
    return false;
  }
  // client_fdを引き出した後、
  // そのクライアントからの接続時に発火するイベントを登録
  struct epoll_event ev;
  ev.data.fd = client_fd;
  ev.events = (EPOLLIN | EPOLLRDHUP);
  int ok = event_handler_->CreateTrigger(ev.data.fd, ev.events);
  if (ok != 0) {
    std::cerr << "failed to set event" << std::endl;
    close(client_fd);
    return false;
  }
  event_handler_->SetCallback(
    ev.data.fd, EPOLLIN, [this](int fd) { event_(fd); });
  event_handler_->SetCallback(
    ev.data.fd, ev.events, [this](int fd) { CloseEvent(fd); });
  return true;
}

/// @brief サーバーのクローズ処理
void
Server::SafeClose()
{
  if (0 < server_fd_) {
    close(server_fd_);
  }
  if (inet0_ != nullptr) {
    freeaddrinfo(inet0_);
  }
  if (hint_ != nullptr) {
    freeaddrinfo(hint_);
  }
}
} // namespace nw::ipv4::tcp
