#ifndef INCLUDED_STD_EXT_VARIANT_TABLE_OPERATOR_IO_HPP
#define INCLUDED_STD_EXT_VARIANT_TABLE_OPERATOR_IO_HPP
// STL
#include <ostream>
// std_ext
#include "type.hpp"

/// @brief std_ext::table型を出力するストリーム演算子
/// @tparam T0 型1
/// @tparam ...Ts 型N
/// @param os 出力ストリーム
/// @param lv 値
/// @return 出力ストリーム
template<typename T0, typename... Ts>
std::ostream&
operator<<(std::ostream& os, const std_ext::table<T0, Ts...>& lv)
{
  if (lv.empty()) {
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
#endif // INCLUDED_STD_EXT_VARIANT_TABLE_OPERATOR_IO_HPP
