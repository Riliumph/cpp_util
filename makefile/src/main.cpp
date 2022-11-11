// STL
#include <iostream>
// original
#include "sub/storage.hpp"
int
main()
{
  Storage storage{};
  storage.Data("data");
  std::cout << storage << std::endl;
  return 0;
}
