#ifndef USER_CONVERTER_HPP
#define USER_CONVERTER_HPP
// 3rd
#include <pqxx/pqxx>
// original
#include "py/map.hpp"
namespace converter {
py::map
Convert2User(pqxx::result&);
}
#endif // USER_CONVERTER_HPP
