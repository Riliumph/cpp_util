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

namespace nw {
namespace ipv4 {
namespace udp {
/// @brief コンストラクタ
/// @param e_handler イベントハンドラ
/// @param port サーバーポート番号
/// @param hint_ IPv4のヒント情報
Server::Server(std::shared_ptr<event::IF::EventHandler> e_handler,
               u_short port,
               struct addrinfo hint)
  : server_fd_{ 0 }
  , event_handler_{ e_handler }
  , port_{ port }
  , inet0_{ new struct addrinfo }
  , hint_{ new struct addrinfo }
  , timeout_{ 0, 0 }
{
  std::cout << "create udp server" << std::endl;
  Hint(hint);
}

/// @brief デストラクタ
Server::~Server()
{
  SafeClose();
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
    ev.data.fd, ev.events, [this](int fd) { event_(fd); });

  std::cout << "start event_handler" << std::endl;
  event_handler_->Run();
  return true;
}

/// @brief サーバーを構築するネットワーク情報のヒントを設定する
/// @param hint ヒント
void
Server::Hint(const struct addrinfo& hint_data)
{
  std::cout << "copy hint" << std::endl;
  hint_->ai_family = hint_data.ai_family;
  hint_->ai_socktype = hint_data.ai_socktype;
  hint_->ai_flags = hint_data.ai_flags;
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
} // namespace udp
} // namespace ipv4
} // namespace nw
