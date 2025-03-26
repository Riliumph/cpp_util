// STL
#include <iomanip>
#include <iostream>
// ORIGINAL
#include "endian.hpp"

int
main(void)
{
  endian::bytes b;
  b.byte4 = 0x12345678;
  std::cout << b.byte4 << std::endl;
  std::cout << b << std::endl;

  b.byte4 = 0;
  b.byte1[0] = 'a'; // 61
  b.byte1[1] = 'A'; // 41
  std::cout << b.byte4 << std::endl;
  std::cout << b << std::endl;

  b.byte4 = 0;
  b.byte1[0] = 0xff; // 61
  b.byte1[1] = 0xdd; // 41
  std::cout << b.byte4 << std::endl;
  std::cout << b << std::endl;
  return 0;
}
