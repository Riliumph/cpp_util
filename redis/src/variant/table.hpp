#ifndef INCLUDE_VARIANT_TABLE_HPP
#define INCLUDE_VARIANT_TABLE_HPP
// STL
#include <map>
#include <utility>
#include <variant>
#include <vector>
// original
#include "operator_io.hpp"
#include "record.hpp"

template<typename T0, typename... Ts>
using table = std::vector<record<T0, Ts...>>;

template<typename T0, typename... Ts>
std::ostream&
operator<<(std::ostream& os, const table<T0, Ts...>& lv)
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
    os << row << std::endl;
  }
  return os;
}
#endif // INCLUDE_VARIANT_TABLE_HPP
