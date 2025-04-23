// STL
#include <cstdio>
#include <cstring>
#include <iostream>
// system
#include <unistd.h>
// 3rd
#include "endian.hpp"
// original
#include "network.h"

void
receive_event(int fd)
{
  const int buffer_size = 1024;
  char buffer[buffer_size];
  auto recv_size = read(fd, buffer, sizeof(buffer));
  std::printf("recv(%ld): %s\n", recv_size, buffer);
}

int
main()
{
  auto pid = getpid();
  std::cout << "server pid: " << pid << std::endl;

  u_short port_no = 80;
  struct addrinfo hint;
  hint.ai_family = AF_INET;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_flags = AI_PASSIVE;
  auto eh = std::make_shared<event::EpollHandler>();
  eh->Timeout(std::nullopt);

  std::cout << "create server..." << std::endl;
  auto srv = nw::ipv4::MakeServer(eh, port_no, hint);
  srv->Event(receive_event);

  std::cout << "establish server..." << std::endl;
  if (srv->Establish() < 0) {
    std::cerr << "failed to establish server" << std::endl;
    return -1;
  }

  std::cout << "start server..." << std::endl;
  srv->Start();
}
