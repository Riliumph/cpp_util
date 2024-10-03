#include "diff.hpp"
// STL
#include <regex>

/// @brief 文字列集合から正規表現で差集合を取る。
/// 対象の集合から必要な集合が存在するかどうかを判定する。
/// 対称差集合ではない。
/// @param required_patterns
/// @param provided_keys
/// @return
std::set<std::string>
set_difference_by_regex(const std::set<std::string>& required_patterns,
                        const std::set<std::string>& data)
{
  std::set<std::string> missing_keys;
  for (const auto& pattern_str : required_patterns) {
    std::regex pattern(pattern_str);
    bool matched = false;
    for (const auto& d : data) {
      // 文字列全体とパターンを比較するためmatchを用いる。
      // std::regex_searchは用いない。
      if (std::regex_match(d, pattern)) {
        matched = true;
        break;
      }
    }
    if (!matched) {
      missing_keys.insert(pattern_str);
    }
  }
  return missing_keys;
}
