#include "redis_api.hpp"
// stl
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
// original
#include "stream_io.hpp"

namespace redis {

void
operate_queue(sw::redis::Redis& client)
{
  std::cout << "# キューの操作" << std::endl;
  std::pair<std::string, std::vector<std::string>> p = {
    "texts", { "nanigashi", "soregashi", "something" }
  };
  try {
    // set data
    std::cout << "set data: " << p.second << std::endl;
    client.lpush(p.first, p.second.begin(), p.second.end());

    // get datas
    std::cout << "get data: ";
    for (size_t i = 0; i < p.second.size(); ++i) {
      auto get_texts = client.rpop(p.first);
      if (get_texts) {
        std::cout << *get_texts << ", ";
      } else {
        std::cout << "(none), ";
      }
    }
    std::cout << std::endl;

    // close
    std::cout << client.del(p.first) << " 件削除" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

}
