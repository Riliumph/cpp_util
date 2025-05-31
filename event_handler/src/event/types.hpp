#ifndef INCLUDE_EVENT_TYPES_HPP
#define INCLUDE_EVENT_TYPES_HPP
// STL
#include <chrono>
#include <functional>

namespace event {
using fd_t = int;
using callback_t = std::function<void(fd_t)>;
using timeout_t = std::chrono::milliseconds;
static constexpr fd_t DISABLED_FD = -1;
} // namespace event
#endif // INCLUDE_EVENT_ABC_TYPES_HPP
