//system
#include <sys/time.h>
//original
#include "user/converter.hpp"
namespace converter {
entity::UserMaps
Convert2UserMaps(pqxx::result& result)
{
  entity::UserMaps user_maps;
  for (const auto& row : result) {
    entity::UserMap user_map;
    for (const auto& col : row) {
      if (col.name() == "id") {
        user_map[col.name()] = std::stoi(col.c_str());
      } else if (col.name() == "nam") {
        user_map[col.name()] = col.c_str();
      } else if (col.name() == "age") {
        user_map[col.name()] = std::stoi(col.c_str());
      }else if(col.name() == "created_at"){
        std::string time_str = col.c_str();
        struct tm t;
        // year
        auto year = std::string(time_str.begin(), time_str.begin() + 4);
        t.tm_year = std::stoi(year);
        // month
        auto month = std::string(time_str.begin()+5, time_str.begin() + 7);
        t.tm_mon = std::stoi(month);
        user_map[col.name()];
      }
    }
  }
}
} // converter
