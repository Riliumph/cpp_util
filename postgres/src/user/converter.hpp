#ifndef USER_CONVERTER_HPP
#define USER_CONVERTER_HPP
// 3rd
#include <pqxx/pqxx>
// original
#include "user/user_map.hpp"
namespace converter {
entity::UserMaps
Covert2UserMap(pqxx::result&);
}
#endif // USER_CONVERTER_HPP
