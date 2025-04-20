#include "json_formatter.hpp"
// STL
#include <chrono>
#include <thread>
// spdlog
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
// 3rd
#include <gtest/gtest.h>

using namespace logger::formatters;
using namespace std::literals::chrono_literals;

namespace {
const char* logger_name = "json_formatter_test_logger";
}

TEST(json_formatter, format_test)
{
  using logger_t = spdlog::logger;
  using formatter_t = JsonFormatter;
  using sink_t = spdlog::sinks::stdout_color_sink_mt;
  auto sink = std::make_shared<sink_t>();
  spdlog::sinks_init_list sinks = { sink };
  auto logger = std::make_shared<logger_t>(logger_name, sinks);
  logger->set_formatter(std::make_unique<formatter_t>());

  SPDLOG_LOGGER_TRACE(logger, "trace message");
  SPDLOG_LOGGER_DEBUG(logger, "debug message");
  SPDLOG_LOGGER_INFO(logger, "info message");
  SPDLOG_LOGGER_WARN(logger, "warn message");
  SPDLOG_LOGGER_ERROR(logger, "error message");
  SPDLOG_LOGGER_CRITICAL(logger, "critical message");
  EXPECT_EQ(true, true);
}
