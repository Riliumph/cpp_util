#ifndef INCLUDED_STD_EXT_BIT_CORE_TYPE_HPP
#define INCLUDED_STD_EXT_BIT_CORE_TYPE_HPP
#include <stdint.h>

namespace std_ext {
namespace bit {
/// @brief 4バイトのデータを扱うための共用体
typedef union
{
  uint32_t byte4;
  uint16_t byte2[2];
  uint8_t byte1[4];
} bytes;
} // namespace bit
} // namespace std_ext
#endif // INCLUDED_STD_EXT_BIT_CORE_TYPE_HPP
