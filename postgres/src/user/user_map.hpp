#ifndef USER_MAP_HPP
#define USER_MAP_HPP
// STL
#include <map>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace entity {
using UserMap_t = std::variant<int, std::string>;
using UserMap = std::map<std::string, UserMap_t>;
using UserMaps = std::vector<UserMap>;
} // entity
#endif // USER_MAP_HPP
