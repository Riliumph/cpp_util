#ifndef INCLUDE_ENDIAN_OPERATOR_IO_HPP
#define INCLUDE_ENDIAN_OPERATOR_IO_HPP
// STL
#include <iostream>
// original
#include "type.hpp"

namespace endian {
std::ostream&
operator<<(std::ostream& os, bytes lv)
{
  std::printf("byte(4): %08X\n", lv.byte4);
  std::printf("byte(2): %08X, %08X\n", lv.byte2[0], lv.byte2[1]);
  std::printf("byte(1): %08X, %08X, %08X, %08X\n",
              lv.byte1[0],
              lv.byte1[1],
              lv.byte1[2],
              lv.byte1[3]);
  return os;
}
}
#endif // INCLUDE_ENDIAN_OPERATOR_IO_HPP