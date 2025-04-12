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
namespace logger {
class Logger
{
public:
  static const char* name;

public:
  Logger(const std::string&);
  ~Logger();

  // Deleted copy constructor and assignment operator
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  // Deleted move constructor and assignment operator
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;

private:
  void Config();
  void SetLevel(const std::string& log_level);

private:
  std::string logger_name_;
  std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> file_sink_;
  std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink_;
  std::shared_ptr<spdlog::logger> logger_;
};
}
#endif // INCLUDE_LOGGER_LOGGER_HPP
