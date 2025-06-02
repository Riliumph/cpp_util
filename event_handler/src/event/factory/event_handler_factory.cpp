#include "event_handler_factory.hpp"
// STL
#include <cassert>

namespace event {
namespace factory {
std::shared_ptr<EventHandlerFactory::event_handler_t>
EventHandlerFactory::MakeEventHandler(std::string event_type)
{
  if (event_type == "epoll") {
    return std::make_shared<epoll_handler_t>();
  }
  if (event_type == "select") {
    return std::make_shared<select_handler_t>();
  }
  assert(false && "not supported type event handler");
  return nullptr;
}
} // namespace event::factory
} // namespace event
