#ifndef INCLUDE_LOGGER_FORMATTERS_JSON_FORMATTER_HPP
#define INCLUDE_LOGGER_FORMATTERS_JSON_FORMATTER_HPP
// STL
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
// json
#include <nlohmann/json.hpp>
// spdlog
#include <spdlog/details/log_msg.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
// fmt
#include <fmt/format.h>
namespace logger {
namespace formatters {
/// @brief JSONフォーマッタ
/// @details spdlogのフォーマッタを継承してJSON形式で出力する
class JsonFormatter : public spdlog::formatter
{
public:
  /// @brief ISO8601形式のタイムスタンプフォーマット
  static constexpr const char* const iso8601 = "%FT%T%z";

public:
  void format(const spdlog::details::log_msg&, spdlog::memory_buf_t&) override;

  std::unique_ptr<spdlog::formatter> clone() const override;
};
} // namespace formatters
} // namespace logger
#endif // INCLUDE_LOGGER_FORMATTERS_JSON_FORMATTER_HPP
