#include "ipv4.hpp"
// STL
#include <iostream>
// system
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace tcp {
namespace ipv4 {
Server::Server()
  : accepting_fd{ 0 }
  , port_no{ 0 }
  ,

{
  FD_ZERO(&fds);
}

Server::~Server() {}

int
Server::Identify(const struct addrinfo hint)
{
  char host_name[NI_MAXHOST];
  char serv_name[NI_MAXSERV];
  errcode = getaddrinfo(NULL, port_no, &hints, &);
  if () {
    (void)fprintf(stderr, "getaddrinfo():%s\n", gai_strerror(errcode));
    return (-1);
  }
}

bool
Server::Establish()
{
  if (server->FD() < 0) {
    // FDが無効の場合だけソケットを作る
    if (server->MakeSocket() < 0) {
      perror("make socket");
      return false;
    }
  }
  if (server->Port() == 0) {
    // port番号が0の時はバインドしない
    return true;
  }
  if (server->Bind() < 0) {
    perror("bind");
    return false;
  }
  return true;
}

bool
Server::Listen()
{
  if (listen(server->FD(), SOMAXCONN) < 0) {
    perror("listen");
    return false;
  }
  return true;
}

/// @brief Accept処理
/// @return fd値
int
Server::Accept()
{
  auto client_fd = accept(server->FD(), 0, 0);
  return client_fd;
}

/// @brief TIME_WAIT状態のポートを再利用する設定を有効化する
/// @return 成功可否
bool
Server::ReuseAddress()
{
  int on = 1;
  auto result =
    setsockopt(accepting_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  if (result < 0) {
    perror("setsockopt(,,SO_REUSEADDR)");
    return false;
  }
  return true;
}

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

void
Server::IP(std::string ip_address)
{
  ip = ip_address;
}

std::string
Server::IP() const
{
  return ip;
}

void
Server::Port(u_short port_no)
{
  this->port_no = port_no;
}

int
Server::Port() const
{
  return port_no;
}

/// @brief Select待ちループ
/// @param fn 反応時に処理する関数オブジェクト
/// @return 成否
bool
Server::LoopBySelect(std::function<bool(int)> fn)
{
  FD_ZERO(&fds);
  FD_SET(server->FD(), &fds);
  while (1) {
    fd_set readable = fds;
    std::printf("wait select ...\n");
    auto updated_fd_num = select(FD_SETSIZE, &readable, 0, 0, &timeout);
    if (updated_fd_num <= 0) {
      perror("select");
      exit(1);
    }
    // Accept
    if (FD_ISSET(server->FD(), &readable)) {
      FD_CLR(server->FD(), &readable);
      std::printf(
        "[PID:%d][FD:%d] accepting connections ...\n", getpid(), server->FD());
      auto client_fd = Accept();
      if (client_fd < 0) {
        perror("accept");
        return false;
      }
      FD_SET(client_fd, &fds);
    }
    // TODO:
    // FD_SETSIZEは上限値であり、接続されたFDの最大値ではないため無駄なループが回っている
    for (auto i = 0; i < FD_SETSIZE; i++) {
      if (FD_ISSET(i, &readable)) {
        std::printf("[FD:%d] accepting connections ...\n", i);
        auto success = fn(i);
        if (!success) {
          close(i);
          FD_CLR(i, &fds);
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
} // namespace ipv4
} // namespace tcp
