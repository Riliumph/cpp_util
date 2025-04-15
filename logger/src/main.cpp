// STL
#include <memory>
// json
#include <nlohmann/json.hpp>
// spdlog
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
// logger
#include "logger.hpp"
int
main()
{
  auto log_system = logger::AppLogger("app.log", 512, 1);
  auto logger = spdlog::get("app_logger");
  APP_TRACE("msg: {}", "trace message");
  APP_DEBUG("msg: {}", "debug message");
  APP_INFO("msg: {}", "info message");
  APP_WARN("msg: {}", "warn message");
  APP_ERROR("msg: {}", "error message");
  APP_CRIT("msg: {}", "critical message");
  return 0;
}
