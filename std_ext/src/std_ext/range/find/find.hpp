#ifndef INCLUDED_STD_EXT_RANGE_FIND_HPP
#define INCLUDED_STD_EXT_RANGE_FIND_HPP
#include "std_ext/range/core/core.hpp"
// STL
#include <algorithm>
#include <iterator>

namespace std_ext {
namespace range {
/// @brief std::find_ifのラッパー関数
/// @details contaienr.begin()と書くのが面倒なので定義
/// @tparam Container コンテナ型
/// @tparam Predicate
/// @param c コンテナ型の転送参照（&&のこと）
/// @param p 述語関数
/// @return マッチした最初のイテレータ
template<typename Container, typename Predicate>
auto
find_if(Container&& c, Predicate p)
  -> std::enable_if_t<is_range<Container>::value, decltype(std::begin(c))>
{
  // &&は右辺値参照と異なり、templateでは転送参照となる。
  // std::begin(c)は、
  // cがstd::vector<int>のようなコンテナであれば通常のiteratorを返し、
  // cがconst std::vector<int>のようなコンテナであればconst_iteratorを返す。
  // 転送参照とstd::begin()の組み合わせにより、1つの実装でコンテナの型に応じたイテレータが返される
  return std::find_if(std::begin(c), std::end(c), p);
}
} // namespace range
} // namespace range
#endif // INCLUDED_STD_EXT_RANGE_FIND_HPP
