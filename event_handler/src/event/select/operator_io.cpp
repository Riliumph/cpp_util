#include "operator_io.hpp"

namespace event {
std::ostream&
operator<<(std::ostream& os, const std::set<int>& fds)
{
  for (const auto& fd : fds) {
    os << fd << " ";
  }
  return os;
}
}
