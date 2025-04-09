// STL
#include <memory>
// json
#include <nlohmann/json.hpp>
// spdlog
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
// logger
#include "logger.hpp"
int
main()
{
  auto logger = spdlog::stdout_color_mt("app_logger");
  logger->set_formatter(std::make_unique<logger::JsonFormatter>());
  logger->set_level(spdlog::level::info);
  logger->debug("debug message");
  logger->info("This is a test message");
  logger->warn("Something might be wrong!");
  logger->error("An error occurred");

  return 0;
}
