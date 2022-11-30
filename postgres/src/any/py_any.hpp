#ifndef ANY_PY_ANY_HPP
#define ANY_PY_ANY_HPP
#include "py/map.hpp"

namespace py {
py::map
Convert2Any(pqxx::result&);
} // namespace py
#endif // ANY_PY_ANY_HPP
