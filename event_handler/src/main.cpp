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
#include "socket.hpp"
#include "stdin.hpp"

int
main()
{
  std::shared_ptr<event::interface::EventHandler> e_handler =
    std::make_shared<event::EpollHandler>();
  if (!e_handler->CanReady()) {
    std::cerr << "failed to create epoll handler" << std::endl;
    return -1;
  }
  stdin_sample(e_handler);
  socket_sample(e_handler);
  e_handler->Run();
  return 0;
}
