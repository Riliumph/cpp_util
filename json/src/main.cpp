// STL
#include <iostream>
// original
#include "data.hpp"

int
main(void)
{
  using json = nlohmann::json;
  Data sample_data = { "hoge", "hoge@email.com", 20 };
  json j = sample_data;
  std::cout << j << std::endl;
  return 0;
}
