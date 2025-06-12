#ifndef INCLUDED_STD_EXT_VARIANT_RECORD_TYPE_HPP
#define INCLUDED_STD_EXT_VARIANT_RECORD_TYPE_HPP
// STL
#include <map>
#include <string>
#include <variant>

namespace std_ext {
/// @brief Python-like record type.
/// @tparam T0 value types
/// @tparam ...Ts
template<typename T0, typename... Ts>
using record = std::map<std::string, std::variant<T0, Ts...>>;
}
#endif // INCLUDED_STD_EXT_VARIANT_RECORD_TYPE_HPP
