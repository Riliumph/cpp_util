#include "operator_io.hpp"
#include <cstdio>

std::ostream&
operator<<(std::ostream& os, endian::bytes& lv)
{
  std::printf("byte(4): 0x%08X\n", lv.byte4);
  std::printf("byte(2): 0x%04X, 0x%04X\n", lv.byte2[0], lv.byte2[1]);
  std::printf("byte(1): 0x%02X, 0x%02X, 0x%02X, 0x%02X\n",
              lv.byte1[0],
              lv.byte1[1],
              lv.byte1[2],
              lv.byte1[3]);
  return os;
}
