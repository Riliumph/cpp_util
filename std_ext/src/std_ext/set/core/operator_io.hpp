#ifndef INCLUDED_STD_EXT_SET_CORE_OPERATOR_IO_HPP
#define INCLUDED_STD_EXT_SET_CORE_OPERATOR_IO_HPP
#include <ostream>
#include <set>

/// @brief 集合型の出力演算子
/// @tparam T 型
/// @param os 出力ストリーム
/// @param lv 値
/// @return 出力ストリーム
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
#endif // INCLUDED_STD_EXT_SET_CORE_OPERATOR_IO_HPP
