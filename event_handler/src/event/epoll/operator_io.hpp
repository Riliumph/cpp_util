#ifndef INCLUDE_EVENT_EPOLL_OPERATOR_IO_HPP
#define INCLUDE_EVENT_EPOLL_OPERATOR_IO_HPP
// STL
#include <ostream>
// event
#include "epoll_types.hpp"

std::ostream&
operator<<(std::ostream&, const event::epoll::event_t&);
#endif // INCLUDE_EVENT_OPERATOR_IO_HPP
