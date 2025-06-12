#ifndef INCLUDED_STD_EXT_CHRONO_CORE_OPERATOR_IO_HPP
#define INCLUDED_STD_EXT_CHRONO_CORE_OPERATOR_IO_HPP
// STL
#include <ostream>
// std_ext
#include "type.hpp"

std::ostream&
operator<<(std::ostream&, const std::chrono::system_clock::time_point&);

/// @brief duractionの出力演算子
/// @tparam Rep 値の型
/// @tparam Period 時間の単位を表す比率型
/// @param os 出力ストリーム
/// @param lv 値
/// @return 出力ストリーム
template<typename Rep, typename Period>
std::ostream&
operator<<(std::ostream& os, const std::chrono::duration<Rep, Period>& lv)
{
  using namespace std_ext::chrono;
  if constexpr (std::ratio_equal_v<Period, nsecs::period>) {
    os << lv.count() << "ns";
  } else if constexpr (std::ratio_equal_v<Period, usecs::period>) {
    os << lv.count() << "us";
  } else if constexpr (std::ratio_equal_v<Period, msecs::period>) {
    os << lv.count() << "ms";
  } else if constexpr (std::ratio_equal_v<Period, secs::period>) {
    os << lv.count() << "s";
  } else if constexpr (std::ratio_equal_v<Period, mins::period>) {
    os << lv.count() << "min";
  } else if constexpr (std::ratio_equal_v<Period, hours::period>) {
    os << lv.count() << "h";
  } else {
    os << lv.count() << "unknown";
  }
  return os;
}
#endif // INCLUDED_STD_EXT_CHRONO_CORE_OPERATOR_IO_HPP
