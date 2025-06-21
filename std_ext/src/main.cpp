// STL
#include <iostream>
#include <string>
// original
#include "std_ext/std_ext.hpp"

using namespace std::literals::string_literals;

int
main(void)
{
  // variant
  std_ext::table<int, double, std::string> a = { {
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
  //  if (std::get<std::string>(a[0]["name"]) == "foo") これが↓で書けるように
  if (a[0]["name"] == "foo"s) {
    std::cout << "True: " << a[0] << std::endl;
  }

  a[0]["id"] = a[0]["id"] * 2;
  std::cout << a[0]["id"] << std::endl;
  return 0;
}
