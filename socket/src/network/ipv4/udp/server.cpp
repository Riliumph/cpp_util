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
Server::Server(u_short port, struct addrinfo hint)
  : SocketServer(port, hint)
  , timeout_{ 0, 0 }
{
  std::cout << "create udp server" << std::endl;
}

/// @brief デストラクタ
Server::~Server() {}

void
Server::EventHandler(std::shared_ptr<event::IF::EventHandler> eh)
{
  event_handler_ = eh;
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

} // namespace udp
} // namespace ipv4
} // namespace nw
