#ifndef INCLUDE_LOGGER_LOGGER_HPP
#define INCLUDE_LOGGER_LOGGER_HPP
// STL
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
// spdlog
#include <spdlog/fmt/fmt.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
// original
#include "formatters.hpp"
#include "sinks.hpp"

// defines
#define APP_TRACE(...)                                                         \
  SPDLOG_LOGGER_TRACE(spdlog::get(logger::AppLogger::name), __VA_ARGS__)
#define APP_DEBUG(...)                                                         \
  SPDLOG_LOGGER_DEBUG(spdlog::get(logger::AppLogger::name), __VA_ARGS__)
#define APP_INFO(...)                                                          \
  SPDLOG_LOGGER_INFO(spdlog::get(logger::AppLogger::name), __VA_ARGS__)
#define APP_WARN(...)                                                          \
  SPDLOG_LOGGER_WARN(spdlog::get(logger::AppLogger::name), __VA_ARGS__)
#define APP_ERROR(...)                                                         \
  SPDLOG_LOGGER_ERROR(spdlog::get(logger::AppLogger::name), __VA_ARGS__)
#define APP_CRIT(...)                                                          \
  SPDLOG_LOGGER_CRITICAL(spdlog::get(logger::AppLogger::name), __VA_ARGS__)

namespace logger {
class AppLogger
{
public:
  static const char* const name;
  static const size_t default_max_file_size;
  static const size_t default_max_files;
#ifdef DEBUG_
  // 挙動テスト用に秒単位でログローテーションを行う
  using time_unit = std::chrono::seconds;
#else
  using time_unit = std::chrono::minutes;
#endif
  using file_sink_t = sinks::clock_synced_file_sink_mt<time_unit>;
  using console_sink_t = spdlog::sinks::stdout_color_sink_mt;
  using formatter_t = formatters::JsonFormatter;

public:
  AppLogger(const std::string&);
  AppLogger(const std::string&, size_t max_file_size, size_t max_files);
  ~AppLogger();

  // Deleted copy constructor and assignment operator
  AppLogger(const AppLogger&) = delete;
  AppLogger& operator=(const AppLogger&) = delete;

  // Deleted move constructor and assignment operator
  AppLogger(AppLogger&&) = delete;
  AppLogger& operator=(AppLogger&&) = delete;

private:
  void Init();
  void Config();
  void SetLevel(const std::string& log_level);

private:
  std::string logger_name_;
  std::shared_ptr<file_sink_t> file_sink_;
  std::shared_ptr<console_sink_t> console_sink_;
  std::shared_ptr<spdlog::logger> logger_;
};
}
#endif // INCLUDE_LOGGER_LOGGER_HPP
