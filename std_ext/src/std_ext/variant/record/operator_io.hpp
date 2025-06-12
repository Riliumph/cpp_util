#ifndef INCLUDED_STD_EXT_VARIANT_RECORD_OPERATOR_IO_HPP
#define INCLUDED_STD_EXT_VARIANT_RECORD_OPERATOR_IO_HPP
#include <ostream>
// std_ext
#include "record.hpp"

/// @brief record型を出力するストリーム演算子
/// @tparam T0 1つ目の型
/// @tparam ...Ts N個目の型
/// @param os 出力ストリーム
/// @param lv 値
/// @return 出力ストリーム
template<typename T0, typename... Ts>
std::ostream&
operator<<(std::ostream& os, const std_ext::record<T0, Ts...>& lv)
{
  for (const auto& col : lv) {
    os << col.second << "\t";
  }
  return os;
}
#endif // INCLUDED_STD_EXT_VARIANT_RECORD_OPERATOR_IO_HPP
