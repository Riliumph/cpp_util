// STL
#include <iostream>
// system
#include <sys/time.h>
// original
#include "user/converter.hpp"
namespace converter {
/// @brief Userテーブルを表現するマップを構築する。
/// Userテーブルとjoinした場合、表現できない
/// @param result Userテーブルのpwqxx
/// @return python形式でUserテーブルを表現したmap
py::map
Convert2User(pqxx::result& result)
{
  py::map users;
  for (const auto& row : result) {
    py::record record;
    for (const auto& col : row) {
      auto name = std::string(col.name());
      if (name == "id") {
        record[name] = std::stoi(col.c_str());
      } else if (name == "name") {
        record[name] = col.c_str();
      } else if (name == "age") {
        record[name] = std::stoi(col.c_str());
      } else if (name == "modify_at") {
        record[name] = col.c_str();
      } else if (name == "created_at") {
        record[name] = col.c_str();
      }
    }
    users.emplace_back(record);
  }
  return users;
}
} // converter
