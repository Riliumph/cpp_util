#ifndef INCLUDE_EVENT_OPERATOR_IO_H
#define INCLUDE_EVENT_OPERATOR_IO_H
// STL
#include <ostream>
// system
#include <sys/epoll.h>

std::ostream&
operator<<(std::ostream&, const struct epoll_event&);

#endif // INCLUDE_EVENT_OPERATOR_IO_H
