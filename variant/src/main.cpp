// STL
#include <iostream>
// original
#include "variant.hpp"

int
main(void)
{
  table<int, double, std::string> a = { {
                                          { "id", 1 },
                                          { "name", "foo" },
                                        },
                                        {
                                          { "id", 2 },
                                          { "name", "bar" },
                                        } };

  // std::variantはboost::variantと違ってストリーム演算子が無いので定義
  std::cout << "print all data" << std::endl;
  std::cout << a << std::endl;

  std::cout << "test operator== " << std::endl;
  //  if (std::get<int>(a[0]["id"]) == 1) これが↓で書けるように
  if (a[0]["id"] == 1) {
    std::cout << "True: " << a[0] << std::endl;
  }
  return 0;
}
