#ifndef INCLUDED_STD_EXT_ENUM_ECAST_HPP
#define INCLUDED_STD_EXT_ENUM_ECAST_HPP
// STL
#include <type_traits>

/// @brief 長いunderlying_typeを省略するためのエイリアス
/// @tparam E 列挙型
/// @param e 列挙型の値
/// @return 列挙型をネイティブ型に変換した値
template<typename E>
constexpr auto
e_cast(E e) -> typename std::underlying_type<E>::type
{
  return static_cast<typename std::underlying_type<E>::type>(e);
}
#endif // INCLUDED_STD_EXT_ENUM_ECAST_HPP
