#ifndef INCLUDE_VALIANT_OPERATOR_IO_HPP
#define INCLUDE_VALIANT_OPERATOR_IO_HPP
// STL
#include <ostream>
#include <variant>
/// @brief value_tを出力するストリーム演算子
/// ref: boost/variant/detail/variant_io.hpp
/// @param os 出力ストリーム
/// @param lv 左辺値
/// @return 出力ストリーム
template<typename T0, class... Ts>
std::ostream&
operator<<(std::ostream& os, const std::variant<T0, Ts...>& lv)
{
  std::visit([&](const auto& x) { os << x; }, lv);
  return os;
}
#endif // INCLUDE_VALIANT_OPERATOR_IO_HPP
