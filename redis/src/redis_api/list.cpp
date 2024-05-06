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
input_list_data(sw::redis::Redis& client)
{
  std::cout << "# list型の操作" << std::endl;
  std::pair<std::string, std::vector<std::string>> p = {
    "texts", { "foo", "bar", "hoge", "fuga" }
  };
  try {
    // set
    std::cout << "set data: " << p.second << std::endl;
    client.rpush(p.first, p.second.begin(), p.second.end());
    // get
    std::vector<std::string> ret_texts;
    client.lrange(p.first, 0, -1, std::back_inserter(ret_texts));
    std::cout << "get data: " << ret_texts << std::endl;
    // close
    std::cout << client.del(p.first) << " 件削除" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void
input_capped_list_data(sw::redis::Redis& client)
{
  std::cout << "# 上限付リスト" << std::endl;
  std::pair<std::string, std::vector<std::string>> p = {
    "texts", { "foo", "bar", "hoge", "fuga" }
  };
  try {
    // set data
    std::cout << "set data: " << p.second << std::endl;
    auto rpush_result = client.rpush(p.first, p.second.begin(), p.second.end());
    std::cout << rpush_result << "件入力" << std::endl;

    // get data
    std::vector<std::string> ret_texts;
    client.lrange(p.first, 0, -1, std::back_inserter(ret_texts));
    std::cout << "get data: " << ret_texts << std::endl;

    std::cout << "## 指定範囲外の要素を削除" << std::endl;
    client.ltrim(p.first, 0, 2);

    // get data
    ret_texts.clear();
    client.lrange(p.first, 0, -1, std::back_inserter(ret_texts));
    std::cout << "get data: " << ret_texts << std::endl;
    // close
    std::cout << client.del(p.first) << " 件削除" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
}
