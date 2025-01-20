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
Server::Server(std::shared_ptr<event::IF::EventHandler> e_handler,
               const u_short port,
               const struct addrinfo hint)
  : server_fd{ 0 }
  , event_handler{ e_handler }
  , ip{ "" }
  , port_{ port }
  , inet0{ new struct addrinfo }
  , hint{ new struct addrinfo }
  , timeout{ 0, 0 }
{
  FD_ZERO(&fds);
  std::fill(client_fds.begin(), client_fds.end(), DISABLE_FD);
  Hint(hint);
}

Server::~Server()
{
  SafeClose();
}

#ifndef ACCESSOR

void
Server::Timeout(struct timeval tm)
{
  timeout = tm;
}

void
Server::Timeout(time_t sec, suseconds_t usec)
{
  timeout.tv_sec = sec;
  timeout.tv_usec = usec;
}

void
Server::Event(event::IF::EventHandler::callback e)
{
  event = e;
}

/// @brief サーバーを立ち上げるための
/// @return
int
Server::Establish()
{
  Identify();
  auto ok = CreateSocket();
  if (ok < 0) {
    return ok;
  }
  ok = ReuseAddress(server_fd);
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

/// @brief Select待ちループ
/// @param fn 反応時に処理する関数オブジェクト
/// @return 成否
bool
Server::Start()
{
  if (!event_handler) {
    std::cerr << "event_handler not set" << std::endl;
    return false;
  }
  if (!event_handler->CanReady()) {
    std::cerr << "event_handler not ready" << std::endl;
    return false;
  }
  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = server_fd;
  auto ok = event_handler->CreateTrigger(ev.data.fd, ev.events);
  if (ok != 0) {
    std::cerr << "failed to set event" << std::endl;
    return false;
  }
  event_handler->SetCallback(
    ev.data.fd, ev.events, [this](int fd) { AcceptEvent(fd); });

  std::cout << "start event_handler" << std::endl;
  event_handler->Run();
  return true;
}

/// @brief サーバーを構築するネットワーク情報のヒントを設定する
/// @param hint ヒント
void
Server::Hint(const struct addrinfo& hint_data)
{
  std::cout << "copy hint" << std::endl;
  hint->ai_family = hint_data.ai_family;
  hint->ai_socktype = hint_data.ai_socktype;
  hint->ai_flags = hint_data.ai_flags;
}

#endif // ACCESSOR

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
  auto err = getaddrinfo(NULL, service_name.data(), hint, &inet0);
  if (err != 0) {
    std::cerr << "getaddrinfo(): " << gai_strerror(err) << std::endl;
    return -1;
  }

  err = getnameinfo(inet0->ai_addr,
                    inet0->ai_addrlen,
                    host_name, // output引数
                    sizeof(host_name),
                    serv_name, // output引数
                    sizeof(serv_name),
                    NI_NUMERICHOST | NI_NUMERICSERV);
  if (err != 0) {
    std::cerr << "getnameinfo(): " << gai_strerror(err) << std::endl;
    freeaddrinfo(inet0);
    return -1;
  }
  printf("identify: %s:%s\n", host_name, serv_name);
  return 0;
}

int
Server::CreateSocket()
{
  // addrinfo型はリンクリストを形成するため、forで対応する
  for (auto info = inet0; info != nullptr; info = info->ai_next) {
    server_fd =
      socket(inet0->ai_family, inet0->ai_socktype, inet0->ai_protocol);
    if (server_fd < 0) {
      perror("make socket");
      continue;
    }
    // bindのチェックまでやった方がいい
  }
  return server_fd;
}

int
Server::AttachAddress()
{
  auto ok = bind(server_fd, inet0->ai_addr, inet0->ai_addrlen);
  if (ok < 0) {
    perror("bind");
    return ok;
  }
  return ok;
}

int
Server::Listen()
{
  auto err = listen(server_fd, QUEUE_SIZE);
  if (err < 0) {
    perror("listen");
    return err;
  }
  return err;
}

/// @brief Accept処理
/// @return fd値
int
Server::Accept()
{
  struct sockaddr_storage from;
  auto len = (socklen_t)sizeof(from);
  auto client_fd = accept(server_fd, (struct sockaddr*)&from, &len);
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

int
Server::CurrentConnection()
{
  auto count = std::count_if(client_fds.begin(), client_fds.end(), [](int fd) {
    return fd != DISABLE_FD;
  });
  return count;
}

int
Server::ControlMaxConnection(const int client_fd)
{
  auto* it = std::find(client_fds.begin(), client_fds.end(), DISABLE_FD);
  if (it == client_fds.end()) {
    return -1;
  }
  *it = client_fd;
  printf("now clients: %d/%ld\n", CurrentConnection(), client_fds.size());
  auto index = std::distance(client_fds.begin(), it);
  return index;
}

bool
Server::CloseEvent(int fd)
{
  std::cout << "Client disconnected: " << fd << std::endl;
  event_handler->EraseCallback(fd);
  event_handler->DeleteTrigger(fd, EPOLLIN);
  close(fd);
  return true;
}

bool
Server::AcceptEvent(int server_fd)
{
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
  int ok = event_handler->CreateTrigger(ev.data.fd, ev.events);
  if (ok != 0) {
    std::cerr << "failed to set event" << std::endl;
    close(client_fd);
    return false;
  }
  event_handler->SetCallback(
    ev.data.fd, EPOLLIN, [this](int fd) { event(fd); });
  event_handler->SetCallback(
    ev.data.fd, ev.events, [this](int fd) { CloseEvent(fd); });
  return true;
}

void
Server::SafeClose()
{
  if (0 < server_fd) {
    close(server_fd);
  }
  if (inet0 != nullptr) {
    freeaddrinfo(inet0);
  }
  if (hint != nullptr) {
    freeaddrinfo(hint);
  }
}
} // namespace nw::ipv4::tcp
