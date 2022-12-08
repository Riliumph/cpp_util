#ifndef UTIL_VALIANT_OPERATOR_HPP
#define UTIL_VALIANT_OPERATOR_HPP
#include <variant>
/// @brief 型Tとstd::variant型の比較オペレータ
/// @tparam T std::variantの一部の型
/// @tparam ...Types std::variantが取り得る型
/// @param t 固定型の変数
/// @param v std::variantの変数
/// @return 比較結果
template<typename T, class... Ts>
inline bool
operator==(const T& t, const std::variant<Ts...>& v)
{
  const T* p = std::get_if<T>(&v);
  if (p) {
    return *p == t;
  }
  return false;
}
/// @brief std::variant型と型Tの比較オペレータ
/// @tparam T std::variantの一部の型
/// @tparam ...Types std::variantが取り得る型
/// @param v std::variantの変数
/// @param t 固定型の変数
/// @return 比較結果
template<typename T, class... Types>
inline bool
operator==(const std::variant<Types...>& v, const T& t)
{
  return t == v;
}
#endif // UTIL_VALIANT_OPERATOR_HPP
