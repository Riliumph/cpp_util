// STL
#include <iostream>
// original
#include "py/map.hpp"

namespace py {
} // namespace py

std::ostream&
operator<<(std::ostream& os, const py::map& lv)
{
  if (lv.size() <= 0) {
    return os;
  }
  // show header
  for (const auto& col : lv.at(0)) {
    os << col.first << "\t";
  }
  os << std::endl;
  // show data
  for (const auto& row : lv) {
    for (const auto& col : row) {
      os << col.second << "\t";
    }
    os << std::endl;
  }
  return os;
}
