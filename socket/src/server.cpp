// STL
#include <algorithm>
#include <iostream>
// POSIX Standard
#include <unistd.h>
// system
#include "network/network.h"

void
receive_event(int fd)
{
  const int buffer_size = 1024;
  char buffer[buffer_size];
  auto recv_size = read(fd, buffer, sizeof(buffer));
  std::printf("recv(%ld): %s\n", recv_size, buffer);
}

int
main(int argc, char* argv[])
{
  if (argc < 5) {
    std::cerr << "引数が足りません" << std::endl;
    return -1;
  }

  auto pid = getpid();
  std::cout << "server pid: " << pid << std::endl;

  auto event = std::string(argv[1]);
  auto protocol = std::string(argv[2]);
  auto ip = std::string(argv[3]);
  auto port = std::string(argv[4]);

  struct addrinfo hint;
  hint.ai_family = AF_INET;
  hint.ai_flags = AI_PASSIVE;
  if (protocol == "tcp") {
    hint.ai_socktype = SOCK_STREAM;
  } else if (protocol == "udp") {
    hint.ai_socktype = SOCK_DGRAM;
  } else {
    std::cerr << "not supported protocol: " << protocol << std::endl;
    return -1;
  }

  auto server_factory = nw::l4::ServerFactory();
  auto srv = server_factory.MakeServer(ip, port, hint);
  if (!srv) {
    std::cerr << "failed to create server" << std::endl;
    return -1;
  }

  auto event_factory = event::factory::EventHandlerFactory();
  auto eh = event_factory.MakeEventHandler(event);
  if (!eh) {
    std::cerr << "failed to create event handler" << std::endl;
    return -1;
  }

  srv->EventHandler(eh);
  srv->Event(receive_event);

  std::cout << "establish server..." << std::endl;
  if (srv->Establish() < 0) {
    std::cerr << "failed to establish server" << std::endl;
    return -1;
  }

  std::cout << "start server..." << std::endl;
  srv->Start();
}
