#ifndef INCLUDE_ADL_MAP_HPP
#define INCLUDE_ADL_MAP_HPP
// STL
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
// 3rd
#include <boost/variant.hpp>

namespace adl {
using value_t = boost::variant<int, std::string>;
using record = std::unordered_map<std::string, value_t>;
using relation = std::vector<record>;

#ifdef ADL_ERROR
// これはADLにより正しく参照されない
std::ostream&
operator<<(std::ostream&, const relation&);
#endif
}

#ifndef ADL_ERROR
// これはADLにより正しく参照される。
std::ostream&
operator<<(std::ostream&, const adl::relation&);
#endif

#endif // INCLUDE_ADL_MAP_HPP
