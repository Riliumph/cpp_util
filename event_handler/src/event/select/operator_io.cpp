#include "operator_io.hpp"

std::ostream&
operator<<(std::ostream& os, const std::set<event::fd_t>& fds)
{
  for (const auto& fd : fds) {
    os << fd << " ";
  }
  return os;
}
