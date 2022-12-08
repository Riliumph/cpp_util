#ifndef UTIL_VARIANT_RECORD_HPP
#define UTIL_VARIANT_RECORD_HPP
// STL
#include <map>
#include <utility>
#include <variant>
#include <vector>
// original
#include "operator_io.hpp"

template<typename T0, typename... Ts>
using record = std::map<std::string, std::variant<T0, Ts...>>;

template<typename T0, typename... Ts>
std::ostream&
operator<<(std::ostream& os, const record<T0, Ts...>& lv)
{
  for (const auto& col : lv) {
    os << col.second << "\t";
  }
  return os;
}
#endif // UTIL_VARIANT_RECORD_HPP
