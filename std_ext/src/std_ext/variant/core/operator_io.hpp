#ifndef INCLUDED_STD_EXT_VARIANT_CORE_OPERATOR_IO_HPP
#define INCLUDED_STD_EXT_VARIANT_CORE_OPERATOR_IO_HPP
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
  std::visit(
    [&](const auto& x) {
      using decayed_t = std::decay_t<decltype(x)>;
      if constexpr (std::is_same_v<decayed_t, std::monostate>) {
        os << "(MONOSTATE)";
      } else {
        os << x;
      }
    },
    lv);
  return os;
}
#endif // INCLUDED_STD_EXT_VARIANT_CORE_OPERATOR_IO_HPP
