#ifndef PY_MAP_HPP
#define PY_MAP_HPP
// STL
#include <map>
#include <ostream>
#include <string>
#include <vector>
// boost
#if 0
#include <variant>
#else
#include <boost/variant.hpp>
#endif

/// @brief Pythonライクなint str doubleなどカラム毎に自由な型を入れれられるmap
namespace py {
#if 0
// 2022/11/1 boost::serializationはstd::variantをエンコードできない。
using value_t = std::variant<int, std::string>;
#else
// boost::serializationはboost::variantをエンコードできる。
using value_t = boost::variant<int, std::string>;
#endif
using record = std::unordered_map<std::string, value_t>;
using map = std::vector<record>;
} // namespace py

std::ostream&
operator<<(std::ostream&, const py::map&);
#endif // PY_MAP_HPP
