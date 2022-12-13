#ifndef INCLUDE_ENUM_CAST_HPP
#define INCLUDE_ENUM_CAST_HPP
// STL
#include <type_traits>

template<typename E>
constexpr auto
e_cast(E e) -> typename std::underlying_type<E>::type
{
  return static_cast<typename std::underlying_type<E>::type>(e);
}
#endif // INCLUDE_ENUM_CAST_HPP
