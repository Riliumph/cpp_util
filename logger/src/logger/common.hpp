#ifndef INCLUDE_LOGGER_COMMON_HPP
#define INCLUDE_LOGGER_COMMON_HPP
// STL
#include <cstdlib>
#include <filesystem>
#include <string>
namespace logger {
/// @brief 環境変数から値を取得する
/// @param key 環境変数名
/// @param default_value 存在しない場合のデフォルト値
/// @return
std::string
get_env_or(const char* key, const std::string& default_value)
{
  auto* value = std::getenv(key);
  if (value == nullptr) {
    return default_value;
  }
  return std::string(value);
}
}
#endif // INCLUDE_LOGGER_COMMON_HPP
