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
  auto log_system = logger::Logger("app_logger", "app.log");
  auto logger = spdlog::get("app_logger");
  logger->debug("debug message");
  logger->info("This is a test message");
  logger->warn("Something might be wrong!");
  logger->error("An error occurred");
  return 0;
}
