#ifndef INCLUDE_EVENT_EPOLL_TYPES_HPP
#define INCLUDE_EVENT_EPOLL_TYPES_HPP
// system
#include <sys/epoll.h>

namespace event {
namespace epoll {
using event_t = struct epoll_event;
} // namespace epoll
} // namespace event
#endif // INCLUDE_EVENT_EPOLL_TYPES_HPP
