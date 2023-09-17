#ifndef INCLUDE_VARIANT_RECORD_HPP
#define INCLUDE_VARIANT_RECORD_HPP
// STL
#include <map>
#include <utility>
#include <vector>
// boost
#include <boost/variant.hpp>

#ifdef BOOST
// 2022/11/1 boost::serializationはstd::variantをエンコードできない。
template<typename T0, typename... Ts>
using record = std::map<std::string, std::variant<T0, Ts...>>;
#else
// boost::serializationはboost::variantをエンコードできる。
template<typename T0, typename... Ts>
using record = std::map<std::string, boost::variant<T0, Ts...>>;
#endif

template<typename T0, typename... Ts>
std::ostream&
operator<<(std::ostream& os, const record<T0, Ts...>& lv)
{
  for (const auto& col : lv) {
    os << col.second << "\t";
  }
  return os;
}
#endif // INCLUDE_VARIANT_RECORD_HPP
