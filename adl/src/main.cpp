// STL
#include <iostream>
// orignal
#include "adl/map.hpp"

int
main()
{
  adl::relation v;
  adl::record r;
  r["id"] = 1;
  r["name"] = "sample";
  r["age"] = 17;
  v.emplace_back(r);

  std::cout << v << std::endl;
  return 0;
}
