#ifndef INCLUDED_STD_EXT_SET_DIFF_HPP
#define INCLUDED_STD_EXT_SET_DIFF_HPP
#include <set>
#include <string>

namespace std_ext {
namespace set {
std::set<std::string>
set_difference_by_regex(const std::set<std::string>&,
                        const std::set<std::string>&);
} // namespace set
} // namespace std_ext
#endif // INCLUDED_STD_EXT_SET_DIFF_HPP
