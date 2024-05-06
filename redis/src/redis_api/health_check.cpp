#include "redis_api.hpp"
// stl
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
// original
#include "stream_io.hpp"

namespace redis {
void
health_check(sw::redis::Redis& client)
{
  std::cout << "#疎通確認" << std::endl;
  try {
    std::cout << client.ping() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
}
