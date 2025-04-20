// STL
#include <memory>
#include <thread>
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
  using namespace std::literals::chrono_literals;
  auto log_system = logger::AppLogger("app.log");
  auto logger = spdlog::get("app_logger");
  APP_TRACE("msg: {}", "trace message");
  APP_DEBUG("msg: {}", "debug message");
  APP_INFO("msg: {}", "info message");
  std::this_thread::sleep_for(1s);
  APP_WARN("msg: {}", "warn message");
  std::this_thread::sleep_for(1s);
  APP_ERROR("msg: {}", "error message");
  std::this_thread::sleep_for(1s);
  APP_CRIT("msg: {}", "critical message");
  return 0;
}
