#ifndef GRADE_PY_GRADE_HPP
#define GRADE_PY_GRADE_HPP
// 3rd
#include <pqxx/pqxx>
// original
#include "py/map.hpp"
namespace py {
py::map
Convert2Grade(pqxx::result&);
} // namespace py
#endif // GRADE_PY_GRADE_HPP
