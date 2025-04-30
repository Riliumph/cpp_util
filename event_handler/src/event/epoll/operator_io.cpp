#include "operator_io.hpp"
#include "types.hpp"

std::ostream&
operator<<(std::ostream& os, const event::epoll::event_t& e)
{
  os << "fd(" << e.data.fd << "), event(" << e.events << ")";
  return os;
}
