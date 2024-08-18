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
Server::Server()
  : server_fd{ 0 }
  , ip{ "" }
  , port_{ 0 }
  , inet0{ nullptr }
  , hint{ nullptr }
{
  FD_ZERO(&fds);
}
Server::Server(const u_short port, struct addrinfo hint)
  : server_fd{ 0 }
  , ip{ "" }
  , port_{ port }
  , inet0{ nullptr }
  , hint{ nullptr }
{
  FD_ZERO(&fds);
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

/// @brief サーバーを構築するネットワーク情報のヒントを設定する
/// @param hint ヒント
void
Server::Hint(struct addrinfo& hint_data)
{ // deep copyを行う
  DeepCopy(&hint_data, &hint);
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
  std::cout << "port_no=" << serv_name << std::endl;
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
  auto client_fd = accept(server_fd, 0, 0);
  if (client_fd < 0) {
    perror("accept");
    return client_fd;
  }
  client_fds.emplace_back(client_fd);
  return client_fd;
}

void
Server::CloseClient(int fd)
{
  client_fds.erase(std::remove(client_fds.begin(), client_fds.end(), fd),
                   client_fds.end());
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
  for (const auto& client_fd : client_fds) {
    CloseClient(client_fd);
  }
}

/// @brief Select待ちループ
/// @param fn 反応時に処理する関数オブジェクト
/// @return 成否
bool
Server::LoopBySelect(std::function<bool(int)> fn)
{
  FD_SET(server_fd, &fds);
  while (1) {
    fd_set readable = fds;
    std::printf("wait select ...\n");
    auto updated_fd_num = select(FD_SETSIZE, &readable, 0, 0, &timeout);
    if (updated_fd_num < 0) {
      perror("select");
      exit(1);
    } else if (updated_fd_num == 0) {
      perror("select timeout");
      continue;
    }
    // Accept
    if (FD_ISSET(server_fd, &readable)) {
      FD_CLR(server_fd, &readable);
      std::printf(
        "[PID:%d][FD:%d] accepting connections ...\n", getpid(), server_fd);
      auto client_fd = Accept();
      if (client_fd < 0) {
        return false;
      }
      FD_SET(client_fd, &fds);
    }
    // TODO:
    // FD_SETSIZEは上限値であり、接続されたFDの最大値ではないため無駄なループが回っている
    for (const auto& client_fd : client_fds) {
      if (FD_ISSET(client_fd, &readable)) {
        std::printf("[FD:%d] accepteding connections ...\n", client_fd);
        auto success = fn(client_fd);
        if (!success) {
          close(client_fd);
          FD_CLR(client_fd, &fds);
        }
      }
    }
  }
}

#ifdef EPOLL
const int EVENT_SIZE = 1024;

bool
Server::LoopByEPoll(std::function<bool(int)> fn)
{
  struct epoll_event evs[EVENT_SIZE];
  auto ep_fd = epoll_create(1);
  epoll_ctl(ep_fd, listen_sock, EPOLLIN | EPOLLOUT | EPOLLET);
  while (1) {
    struct epoll_event ev;
    std::printf("wait epoll ...\n");
    auto updated_fd_num = epoll_wait(ep_fd, evs, sizeof(evs), -1);
    if (updated_fd_num <= 0) {
      perror("epoll");
      exit(1);
    }
    // Accept
    auto exist =
      std::any_of(std::begin(evs), std::end(evs), [this](const auto& e) {
        return e.data.fd == server->FD();
      });
    if (exist) {
      std::printf(
        "[PID:%d][FD:%d] accepting connections ...\n", getpid(), server->FD());
      auto client_fd = Accept();
      if (client_fd < 0) {
        perror("accept");
        exit(1);
      }
      setnonblocking(client_fd);
      ev.events = EPOLLIN | EPOLLET;
      ev.data.fd = client_fd;
      if (epoll_ctl(ep_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
        fprintf(stderr, "epoll set insertion error: fd=%d\n", client_fd);
        return false;
      }
    }
    // TODO:
    // FD_SETSIZEは上限値であり、接続されたFDの最大値ではないため無駄なループが回っている
    for (auto i = 0; i < updated_fd_num; ++i) {
      if (evs[i].data.fd == server->FD()) {

        fn(evs[i].data.fd);
      }
    }
  }
}
#endif // EPOLL
}
