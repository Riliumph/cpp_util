#include "ipv4.hpp"
// STL
#include <iostream>
#include <stdlib.h>
// system
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Server::Server()
  : server_fd{ 0 }
  , ip{ "" }
  , port_{ 0 }
  , inet0{ nullptr }
{
  FD_ZERO(&fds);
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

struct timeval
Server::Timeout() const
{
  return timeout;
}

// void
// Server::IP(std::string ip_address)
// {
//   ip = ip_address;
// }

std::string
Server::IP() const
{
  return ip;
}

void
Server::Port(u_short port_no)
{
  port_ = port_no;
}

int
Server::Port() const
{
  return port_;
}

#endif // ACCESSOR

/// @brief ヒント情報からアドレス情報を決定する
/// @param hint 不完全なaddrinfo構造体
/// @param service_name サービス名
/// Linux: /etc/services
/// Windows: C:\Windows\system32\drivers\etc\services
/// @return
int
Server::Identify(struct addrinfo hint, std::string service_name)
{
  if (service_name.empty()) {
    service_name = std::to_string(port_);
  }
  // ヒント変数からアドレスを決定し、inet0変数へ設定
  // 第１引数は、eth0とloの両方から受信できるようにNULLを指定する。
  // 第２引数は、サーバーにおいて短命ポートは使いにくいので指定する。
  // 第３引数は、addrinfo型のinet0はリンクリストを形成することに注意。
  auto err = getaddrinfo(NULL, service_name.data(), &hint, &inet0);
  if (err != 0) {
    std::cerr << "getaddrinfo(): " << *gai_strerror(err) << std::endl;
    return -1;
  }

  err = getnameinfo(inet0->ai_addr,
                    inet0->ai_addrlen,
                    host_name,
                    sizeof(host_name),
                    serv_name,
                    sizeof(serv_name),
                    NI_NUMERICHOST | NI_NUMERICSERV);
  if (err != 0) {
    std::cerr << "getnameinfo(): " << *gai_strerror(err) << std::endl;
    freeaddrinfo(inet0);
    return -1;
  }
  return 0;
}

int
Server::Socket()
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
Server::Bind()
{
  auto err = ReuseAddress();
  if (err < 0) {
    return err;
  }
  err = bind(server_fd, inet0->ai_addr, inet0->ai_addrlen);
  if (err < 0) {
    perror("bind");
    return err;
  }
  return err;
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

/// @brief TIME_WAIT状態のポートを再利用する設定を有効化する
/// @return 成功可否
/// bind前に実行すること
int
Server::ReuseAddress()
{
  int on = 1;
  auto result =
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  if (result < 0) {
    perror("setsockopt(,,SO_REUSEADDR)");
    return result;
  }
  return result;
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
  FD_ZERO(&fds);
  FD_SET(server_fd, &fds);
  while (1) {
    fd_set readable = fds;
    std::printf("wait select ...\n");
    auto updated_fd_num = select(FD_SETSIZE, &readable, 0, 0, &timeout);
    if (updated_fd_num <= 0) {
      perror("select");
      exit(1);
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
        std::printf("[FD:%d] accepting connections ...\n", client_fd);
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
