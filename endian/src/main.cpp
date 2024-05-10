// STL
#include <iomanip>
#include <iostream>
// ORIGINAL
#include "endian.hpp"

typedef union
{
  uint32_t byte4;
  uint16_t byte2[2];
  uint8_t byte1[4];
} bytes;

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

int
main(void)
{
  bytes b;
  b.byte4 = 0x12345678;
  std::cout << b << std::endl;
  return 0;
}
