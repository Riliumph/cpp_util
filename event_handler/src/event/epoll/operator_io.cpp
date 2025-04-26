#include "operator_io.hpp"

std::ostream&
operator<<(std::ostream& os, const struct epoll_event& e)
{
  os << "fd(" << e.data.fd << "), event(" << e.events << ")";
  return os;
}
