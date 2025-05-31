#ifndef INCLUDE_EVENT_TYPES_HPP
#define INCLUDE_EVENT_TYPES_HPP
// STL
#include <chrono>
#include <functional>
// SYSTEM
#include <sys/epoll.h>

namespace event {
using fd_t = int;
using event_id_t = decltype(epoll_event{}.events);
using callback_t = std::function<void(fd_t)>;
using timeout_t = std::chrono::milliseconds;
static constexpr fd_t DISABLED_FD = -1;
} // namespace event
#endif // INCLUDE_EVENT_ABC_TYPES_HPP
