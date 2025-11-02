#ifndef INCLUDED_STD_EXT_RANGE_CORE_CONSTRAINT_HPP
#define INCLUDED_STD_EXT_RANGE_CORE_CONSTRAINT_HPP
// STL
#include <iterator>
#include <type_traits>
#include <utility>

namespace std_ext {
namespace range {
template<typename T, typename = void>
struct is_range : std::false_type
{};

template<typename T>
struct is_range<T,
                std::void_t<decltype(std::begin(std::declval<T&>())),
                            decltype(std::end(std::declval<T&>()))>>
  : std::true_type
{};
} // namespace range
} // namespace std_ext

#endif // INCLUDED_STD_EXT_RANGE_CORE_CONSTRAINT_HPP
