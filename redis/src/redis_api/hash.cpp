#include "redis_api.hpp"
// stl
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
// 3rd
#include <nlohmann/json.hpp>
// original
#include "variant.hpp"

namespace redis {
void
operate_hash(sw::redis::Redis& client)
{
  using json = nlohmann::json;
  std::cout << "# ハッシュ操作" << std::endl;
  std::string key = "hash";
  std::unordered_map<std::string, std::string> val = {
    { "username", "hoge" }, { "e-mail", "hoge@mail.com" }
  };

  try {
    // set
    std::cout << "set hash data" << std::endl;
    client.hmset(key, val.begin(), val.end());
    // get
    std::unordered_map<std::string, std::string> o;
    client.hgetall(key, std::inserter(o, o.begin()));
    for (const auto& v : o) {
      std::cout << "{" << v.first << "," << v.second << "}, " << std::endl;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

/// @brief json -> unordered_map -> Redis(Hash)
/// @param client
void
operate_json1(sw::redis::Redis& client)
{
  using json = nlohmann::json;
  std::cout << "# json操作" << std::endl;
  std::string key = "json";
  json val = { { "username", "hoge" },
               { "e-mail", "hoge@mail.com" },
               { "age", 17 } };
  std::unordered_map<std::string, std::string> umap;
  for (const auto& [key, val] : val.items()) {
    std::stringstream ss;
    ss << val;
    umap[key] = ss.str();
  }

  try {
    // set
    std::cout << "set hash data" << std::endl;
    client.hmset(key, umap.begin(), umap.end());
    // get
    std::unordered_map<std::string, std::string> o;
    client.hgetall(key, std::inserter(o, o.begin()));
    for (const auto& v : o) {
      std::cout << "{" << v.first << "," << v.second << "}, " << std::endl;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
}
