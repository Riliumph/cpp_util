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
  // bit
  std_ext::bit::bytes b;
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

  // enum
  enum struct COLOR
  {
    RED,
    BLUE,
    GREEN,
    QTY
  };

  COLOR colors[e_cast(COLOR::QTY)] = { COLOR::RED, COLOR::BLUE, COLOR::GREEN };

  for (auto color : colors) {
    switch (color) {
      case COLOR::RED:
        std::cout << "RED" << std::endl;
        break;
      case COLOR::BLUE:
        std::cout << "BLUE" << std::endl;
        break;
      case COLOR::GREEN:
        std::cout << "GREEN" << std::endl;
        break;
      default:
        std::cout << "UNDEFINED" << std::endl;
        break;
    }
  }
  return 0;
}
