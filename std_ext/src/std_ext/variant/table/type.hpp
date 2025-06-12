#ifndef INCLUDED_STD_EXT_VARIANT_TABLE_TYPE_HPP
#define INCLUDED_STD_EXT_VARIANT_TABLE_TYPE_HPP
// STL
#include <vector>
// std_ext
#include "std_ext/variant/core/core.hpp"
#include "std_ext/variant/record/record.hpp"

namespace std_ext {
/// @brief テーブル型
/// @tparam T0 型1
/// @tparam ...Ts 型N
template<typename T0, typename... Ts>
using table = std::vector<record<T0, Ts...>>;
}
#endif // INCLUDED_STD_EXT_VARIANT_TABLE_TYPE_HPP
