// STL
#include <algorithm>
#include <iostream>
// POSIX Standard
#include <unistd.h>
// original
#include "network.h"

std::shared_ptr<event::abc::EventHandler>
CreateEventHandler()
{
  return std::make_shared<event::epoll::EpollHandler>();
}

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
  if (argc < 4) {
    std::cerr << "引数が足りません" << std::endl;
    return -1;
  }

  auto pid = getpid();
  std::cout << "server pid: " << pid << std::endl;
  std::vector<std::string> supported_protocols = { "udp", "tcp" };
  auto protocol = std::string(argv[1]);
  auto ip = std::string(argv[2]);
  auto port = std::string(argv[3]);

  auto is_supported =
    std::any_of(supported_protocols.begin(),
                supported_protocols.end(),
                [protocol](const auto& v) { return v == protocol; });
  if (!is_supported) {
    std::cerr << "not supported protocol: " << protocol << std::endl;
    return -1;
  }

  struct addrinfo hint;
  hint.ai_family = AF_INET;
  hint.ai_socktype = protocol == "udp" ? SOCK_DGRAM : SOCK_STREAM;
  hint.ai_flags = AI_PASSIVE;
  auto eh = CreateEventHandler();
  eh->Timeout(std::nullopt);

  std::cout << "create server..." << std::endl;
  auto srv = nw::l4::MakeServer(ip, port, hint);
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
