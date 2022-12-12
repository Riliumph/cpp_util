#ifndef UTIL_VALIANT_OPERATOR_HPP
#define UTIL_VALIANT_OPERATOR_HPP
#include <variant>
/// @brief 型Tとstd::variant型の比較演算子
/// @tparam T std::variantの型判定用の型情報
/// @tparam ...Types std::variantが取り得る型
/// @param t 固定型の変数
/// @param v std::variantの変数
/// @return 比較結果
template<typename T, class... Ts>
inline bool
operator==(T t, std::variant<Ts...> v)
{
  const T* p = std::get_if<T>(&v);
  if (p == nullptr) {
    return false;
  }
  return *p == t;
}

/// @brief std::variant型と型Tの比較演算子
/// @tparam T std::variantの型判定用の型
/// @tparam ...Types std::variantが取り得る型
/// @param v std::variantの変数
/// @param t 固定型の変数
/// @return 比較結果
template<typename T, class... Ts>
inline bool
operator==(std::variant<Ts...> v, T t)
{
  return t == v;
}

/// @brief std::variant型と型Tの加算演算子
/// @tparam T std::variantの型判定用の型
/// @tparam ...Types std::variantが取り得る型
/// @param v 固定型の右辺値
/// @param t std::variantの変数
/// @return std::variantの変数
template<typename T, class... Ts>
inline std::variant<Ts...>
operator+(std::variant<Ts...>& v, T&& t)
{
  T* p = std::get_if<T>(&v);
  if (p == nullptr) {
    static_assert(std::__detail::__variant::__exactly_once<T, Ts...>,
                  "T should occur for exactly once in alternatives");
    return v;
  }
  return *p + t;
}

/// @brief std::variant型と型Tの加算演算子
/// @tparam T std::variantの型判定用の型
/// @tparam ...Types std::variantが取り得る型
/// @param v 固定型の右辺値
/// @param t std::variantの変数
/// @return std::variantの変数
template<typename T, class... Ts>
inline std::variant<Ts...>
operator+(T&& t, std::variant<Ts...>& v)
{
  return v + t;
}

/// @brief std::variant型と型Tの減算演算子
/// @tparam T std::variantの型判定用の型
/// @tparam ...Types std::variantが取り得る型
/// @param v 固定型の右辺値
/// @param t std::variantの変数
/// @return std::variantの変数
template<typename T, class... Ts>
inline std::variant<Ts...>
operator-(std::variant<Ts...>& v, T&& t)
{
  T* p = std::get_if<T>(&v);
  if (p == nullptr) {
    return v;
  }
  return *p - t;
}

/// @brief std::variant型と型Tの減算演算子
/// @tparam T std::variantの型判定用の型
/// @tparam ...Types std::variantが取り得る型
/// @param v 固定型の右辺値
/// @param t std::variantの変数
/// @return std::variantの変数
template<typename T, class... Ts>
inline std::variant<Ts...>
operator-(T&& t, std::variant<Ts...>& v)
{
  return v - t;
}

/// @brief std::variant型と型Tの乗算演算子
/// @tparam T std::variantの型判定用の型
/// @tparam ...Types std::variantが取り得る型
/// @param v 固定型の右辺値
/// @param t std::variantの変数
/// @return std::variantの変数
template<typename T, class... Ts>
inline std::variant<Ts...>
operator*(std::variant<Ts...>& v, T&& t)
{
  T* p = std::get_if<T>(&v);
  if (p == nullptr) {
    return v;
  }
  return *p * t;
}

/// @brief std::variant型と型Tの乗算演算子
/// @tparam T std::variantの型判定用の型
/// @tparam ...Types std::variantが取り得る型
/// @param v 固定型の右辺値
/// @param t std::variantの変数
/// @return std::variantの変数
template<typename T, class... Ts>
inline std::variant<Ts...>
operator*(T&& t, std::variant<Ts...>& v)
{
  return v - t;
}

/// @brief std::variant型と型Tの乗算演算子
/// @tparam T std::variantの型判定用の型
/// @tparam ...Types std::variantが取り得る型
/// @param v 固定型の右辺値
/// @param t std::variantの変数
/// @return std::variantの変数
template<typename T, class... Ts>
inline std::variant<Ts...>
operator/(std::variant<Ts...>& v, T&& t)
{
  T* p = std::get_if<T>(&v);
  if (p == nullptr) {
    return v;
  }
  return *p / t;
}

/// @brief std::variant型と型Tの乗算演算子
/// std::string * 3など、型が違う演算はサポートしていない。
/// @tparam T std::variantの型判定用の型
/// @tparam ...Types std::variantが取り得る型
/// @param v 固定型の右辺値
/// @param t std::variantの変数
/// @return std::variantの変数
template<typename T, class... Ts>
inline std::variant<Ts...>
operator/(T&& t, std::variant<Ts...>& v)
{
  T* p = std::get_if<T>(&v);
  if (p == nullptr) {
    return v;
  }
  return t / *p;
}
#endif // UTIL_VALIANT_OPERATOR_HPP
