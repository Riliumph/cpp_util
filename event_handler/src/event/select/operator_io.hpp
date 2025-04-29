#ifndef INCLUDE_EVENT_SELECT_OPERATOR_IO_HPP
#define INCLUDE_EVENT_SELECT_OPERATOR_IO_HPP
#include "select_handler.hpp"
// STL
#include <ostream>
namespace event {
std::ostream&
operator<<(std::ostream& os, const std::set<int>& fds);
}

#endif // INCLUDE_EVENT_SELECT_OPERATOR_IO_HPP
