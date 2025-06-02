#ifndef INCLUDE_ENDIAN_REVERSE_HPP
#define INCLUDE_ENDIAN_REVERSE_HPP
#include <algorithm>
#include <type_traits>

namespace endian {

extern void* enabler;

template<bool B, typename T = void>
using enabler_if = typename std::enable_if<B, T>::type*&;

/// @brief Reverse endian (big<->little)
/// little endian variable changes big endian
/// big endian variable changes little endian
/// allowed argument is only SCALAR TYPE
/// @tparam T typename
/// @tparam is only scalar
/// @tparam is not pointer
/// @param value [i] Convert the target
/// @return trans endian value
template<typename T,
         enabler_if<std::is_scalar<T>::value> = enabler,
         enabler_if<!std::is_pointer<T>::value> = enabler>
T
Reverse(T value)
{
  auto* begin = reinterpret_cast<char*>(&value);
  auto* end = begin + sizeof(T);
  std::reverse(begin, end);
  return value;
}

}
#endif // INCLUDE_ENDIAN_REVERSE_HPP
