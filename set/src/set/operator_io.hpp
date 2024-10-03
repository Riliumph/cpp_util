#ifndef INCLUDE_SET_OPERATOR_IO_HPP
#define INCLUDE_SET_OPERATOR_IO_HPP
#include <ostream>
#include <set>
template<typename T>
std::ostream&
operator<<(std::ostream& os, const std::set<T>& lv)
{
  for (auto it = lv.begin(); it != lv.end(); ++it) {
    if (it != lv.begin()) {
      os << ", ";
    }
    os << *it;
  }
  return os;
}
#endif // INCLUDE_SET_OPERATOR_IO_HPP
