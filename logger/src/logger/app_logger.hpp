#ifndef INCLUDE_LOGGER_LOGGER_HPP
#define INCLUDE_LOGGER_LOGGER_HPP
// STL
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
// spdlog
#include <spdlog/fmt/fmt.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
// original
#include "json_formatter.hpp"
#include "timestamp_rotating_file_sink.hpp"

namespace logger {
class AppLogger
{
public:
  static const char* name;
  static const size_t default_max_file_size;
  static const size_t default_max_files;
  using file_sink_t = timestamp_rotating_file_sink_mt;
  using console_sink_t = spdlog::sinks::stdout_color_sink_mt;
  using formatter_t = JsonFormatter;

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
