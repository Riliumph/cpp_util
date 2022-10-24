// STL
#include <type_traits>

template<typename E>
constexpr auto
e_cast(E e) -> typename std::underlying_type<E>::type
{
  return static_cast<typename std::underlying_type<E>::type>(e);
}
