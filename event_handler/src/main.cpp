// STL
#include <iostream>
// event
#include "event/event.hpp"
// sample
#include "signal.hpp"
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
  auto factory = event::factory::EventHandlerFactory();
  auto e_handler = factory.MakeEventHandler(handler_type);
  if (!e_handler->CanReady()) {
    std::cerr << "failed to create epoll handler" << std::endl;
    return -1;
  }
  signal_sample(e_handler);
  stdin_sample(e_handler);
  socket_sample(e_handler, handler_type);
  e_handler->Run();
  return 0;
}
