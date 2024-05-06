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
set_data(sw::redis::Redis& client)
{
  std::cout << "# 通常の操作" << std::endl;
  std::pair<std::string, double> p = { "double_value", 1.2345678912345678 };
  try {
    {
      // set
      std::cout << "set data: " << p.second << std::endl;
      std::stringstream ss;
      ss << std::setprecision(20) << p.second;
      client.set(p.first, ss.str());
    }
    {
      // get
      std::cout << "get data: ";
      auto ret = client.get(p.first); // std::optional型で返ってくる
      if (!ret) {
        std::cerr << "(none)" << std::endl;
      } else {
        std::cout << *ret << std::endl;
      }
    }
    // close
    std::cout << client.del(p.first) << "件削除" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void
set_data_with_expiration(sw::redis::Redis& client)
{
  std::cout << "# 期限付きデータの操作" << std::endl;
  std::pair<std::string, std::string> p = { "expired_key", "expired_val" };
  auto sleep_time = std::chrono::seconds(3);
  try {
    // set
    std::cout << "set data: " << p.second << std::endl;
    client.set(p.first, p.second, sleep_time); // 3sec後に自動削除
    std::cout << "waiting... " << std::endl;
    std::this_thread::sleep_for(sleep_time);
    // get
    auto ret = client.get(p.first); // std::optional型で返ってくる
    if (!ret) {
      std::cout << "自動削除されて取得できない" << std::endl;
    } else {
      std::cerr << "見つかると消えてない" << std::endl;
    }
    // auto del_result = client.del(p.first); 自動削除される
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

}
