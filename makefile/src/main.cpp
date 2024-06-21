// STL
#include <iostream>
// original
#include "sub/calc.hpp"
#include "sub/storage.hpp"
int
main()
{
  Storage storage("data");
  std::cout << storage << std::endl;
  std::cout << Sum(1, 2) << std::endl;
  std::cout << Subtract(1, 2) << std::endl;
  return 0;
}
