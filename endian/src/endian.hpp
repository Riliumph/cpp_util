#ifndef INCLUDE_ENDIAN_HPP
#define INCLUDE_ENDIAN_HPP
#include <algorithm>
#include <type_traits>
extern void* enabler;

template<bool B, typename T = void>
using enabler_if = typename std::enable_if<B, T>::type*&;

/**
 * @brief Reverse endian (big<->little)
 *
 * little endian variable changes big endian
 * big endian variable changes little endian
 * allowed argument is only SCALAR TYPE
 * @param [i] value Convert the target
 * @return trans endian value
 */
template<typename T,
         enabler_if<std::is_scalar<T>::value> = enabler,
         enabler_if<!std::is_pointer<T>::value> = enabler>
T
ReverseEndian(T value)
{
  auto begin = reinterpret_cast<char*>(&value);
  auto end = begin + sizeof(T);
  std::reverse(begin, end);
  return value;
}

/**
 * @brief Machine's endian checker
 *
 * if machine's endian is little, return true.
 * but, PDP endian is non-correspondence
 * @retval true LITTLE
 * @retval false maybe BIG
 */
bool
IsLittleEndian()
{
#ifdef __LITTLE_ENDIAN__
  return true;
#elif __BIG_ENDIAN__
  return false;
#else
  int t = 1;
  return *reinterpret_cast<char*>(&t) == 1;
#endif
}
#endif // INCLUDE_ENDIAN_HPP
