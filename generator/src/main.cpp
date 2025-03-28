#include "generator.hpp"
#include <functional>
#include <iostream>
#include <iterator>
int
main()
{
#if 0
  g = generator()
  print(next(g))  # "Start", 1
  print(next(g))  # "Middle", 2
  print(next(g))  # "End"
#endif
  auto gen = Generator();
  std::cout << "=== increment sample ===" << std::endl;
  auto it = gen.begin();
  std::cout << *it << std::endl;
  std::cout << *(++it) << std::endl;
  std::cout << *(it++) << std::endl;
  std::cout << std::endl;
  std::cout << "=== std::next() sample ===" << std::endl;
  // std::nextを使う場合
  gen = Generator();
  it = gen.begin();
  std::cout << *it << std::endl;
  it = std::next(it);
  std::cout << *it << std::endl;
  it = std::next(it);
  std::cout << *it << std::endl;

  return 0;
}
