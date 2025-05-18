#ifndef EVENT_FACTORY_FACTORY_HPP
#define EVENT_FACTORY_FACTORY_HPP
// STL
#include <memory>
#include <string>
// original
#include "event/abc/event_handler.hpp"
#include "event/epoll/epoll_handler.hpp"
#include "event/select/select_handler.hpp"

namespace event {
namespace factory {
class EventHandlerFactory
{
public:
  using event_handler_t = event::abc::EventHandler;
  using epoll_handler_t = event::epoll::EpollHandler;
  using select_handler_t = event::select::SelectHandler;

public:
  EventHandlerFactory() = default;
  ~EventHandlerFactory() = default;

  std::shared_ptr<event_handler_t> MakeEventHandler(std::string event_type);
};
} // namespace factory
} // namespace event
#endif // EVENT_FACTORY_FACTORY_HPP
