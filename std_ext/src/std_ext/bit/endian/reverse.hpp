#ifndef INCLUDED_STD_EXT_BIT_ENDIAN_REVERSE_HPP
#define INCLUDED_STD_EXT_BIT_ENDIAN_REVERSE_HPP
#include <algorithm>
#include <type_traits>

namespace std_ext {
namespace endian {
/// @brief Reverse endian (big<->little)
/// little endian variable changes big endian
/// big endian variable changes little endian
/// allowed argument is only SCALAR TYPE
/// @tparam T typename
/// @tparam is only scalar
/// @tparam is not pointer
/// @param value [i] Convert the target
/// @return trans endian value
template<typename T>
T
Reverse(T value)
{
  static_assert(std::is_scalar_v<T>, "T must be scalar");
  static_assert(!std::is_pointer_v<T>, "T must not be pointer");
  auto* begin = reinterpret_cast<char*>(&value);
  auto* end = begin + sizeof(T);
  std::reverse(begin, end);
  return value;
}
} // namespace endian
} // namespace std_ext
#endif // INCLUDED_STD_EXT_BIT_ENDIAN_REVERSE_HPP
