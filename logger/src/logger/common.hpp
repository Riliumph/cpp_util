#ifndef INCLUDE_LOGGER_COMMON_HPP
#define INCLUDE_LOGGER_COMMON_HPP
// STL
#include <cstdlib>
#include <filesystem>
#include <string>
namespace logger {
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
