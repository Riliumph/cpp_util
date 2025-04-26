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
Server::Server(const std::string& ip,
               const std::string& port,
               struct addrinfo hint)
  : SocketServer(ip, port, hint)
{
  std::fill(client_fds_.begin(), client_fds_.end(), DISABLE_FD);
}

/// @brief デストラクタ
Server::~Server() {}

/// @brief サーバーを立ち上げる関数
/// @return 成否
int
Server::Establish()
{
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
  // そのクライアントからのデータ受信時に発火するイベントを登録
  struct epoll_event ev;
  ev.data.fd = client_fd;
  ev.events = (EPOLLIN | EPOLLRDHUP);
  auto ok = event_handler_->CreateTrigger(ev.data.fd, ev.events);
  if (ok != 0) {
    std::cerr << "failed to set event" << std::endl;
    close(client_fd);
    return false;
  }
  event_handler_->SetCallback(
    ev.data.fd, EPOLLIN, [this](int fd) { event_(fd); });
  // 切断情報の場合のイベントを登録
  event_handler_->SetCallback(
    ev.data.fd, ev.events, [this](int fd) { CloseEvent(fd); });
  return true;
}

} // namespace nw::ipv4::tcp
