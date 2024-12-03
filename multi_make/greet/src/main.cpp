// STL
#include <iostream>
// LIB
#include "human.hpp"

int
main()
{
  std::cout << "start" << std::endl;
  auto jp = human::JP();
  jp.greet();
  auto us = human::US();
  us.greet();
  return 0;
}
