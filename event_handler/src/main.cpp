// STL
#include <iostream>
#include <memory>
// Linux
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
// original
#include "event.hpp"
#include "factory.hpp"
#include "socket.hpp"
#include "stdin.hpp"

int
main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "引数が足りません" << std::endl;
    return -1;
  }
  auto handler_type = std::string(argv[1]);
  auto e_handler = CreateEventHandler(handler_type);
  if (!e_handler->CanReady()) {
    std::cerr << "failed to create epoll handler" << std::endl;
    return -1;
  }
  stdin_sample(e_handler);
  socket_sample(e_handler);
  e_handler->Run();
  return 0;
}
