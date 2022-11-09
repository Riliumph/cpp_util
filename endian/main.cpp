// STL
#include <iomanip>
#include <iostream>
// ORIGINAL
#include "endian.hpp"

int
main(void)
{
  int i = 0x12345678;
  std::cout << std::hex << i << std::endl;
  if (IsLittleEndian()) {
    std::cout << std::hex << ReverseEndian(i) << std::endl;
  }
  return 0;
}
