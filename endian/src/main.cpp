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
  std::cout << b << std::endl;
  return 0;
}
