#ifndef USER_PY_USER_HPP
#define USER_PY_USER_HPP
// 3rd
#include <pqxx/pqxx>
// original
#include "py/map.hpp"
namespace py {
py::map
Convert2User(pqxx::result&);
}
#endif // USER_PY_USER_HPP
