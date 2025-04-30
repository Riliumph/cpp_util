#ifndef INCLUDE_ENDIAN_TYPE_HPP
#define INCLUDE_ENDIAN_TYPE_HPP
#include <stdint.h>

namespace endian {
/// @brief 4バイトのデータを扱うための共用体
typedef union
{
  uint32_t byte4;
  uint16_t byte2[2];
  uint8_t byte1[4];
} bytes;
}
#endif // INCLUDE_ENDIAN_TYPE_HPP
