#include "clock_synced_file_sink.hpp"
// STL
#include <chrono>
#include <thread>
// spdlog
#include <spdlog/spdlog.h>
// 3rd
#include <gtest/gtest.h>

using namespace logger;
using namespace logger::sinks;
using namespace std::literals::chrono_literals;

namespace {
const char* filename = "clock_synced_file_sink.log";
const char* logger_name = "timely_rotating_file_sink_test_logger";
}

TEST(clock_synced_file_sink, sec)
{
  using time_unit = std::chrono::seconds;
  using logger_t = spdlog::logger;
  using file_sink_t = clock_synced_file_sink_mt<time_unit>;
  auto file_sink = std::make_shared<file_sink_t>(filename);
  spdlog::sinks_init_list sinks = { file_sink };
  auto logger = std::make_shared<logger_t>(logger_name, sinks);

  SPDLOG_LOGGER_TRACE(logger, "trace message");
  std::this_thread::sleep_for(1s);
  SPDLOG_LOGGER_DEBUG(logger, "debug message");
  std::this_thread::sleep_for(1s);
  SPDLOG_LOGGER_INFO(logger, "info message");
  std::this_thread::sleep_for(1s);
  SPDLOG_LOGGER_WARN(logger, "warn message");
  std::this_thread::sleep_for(1s);
  SPDLOG_LOGGER_ERROR(logger, "error message");
  std::this_thread::sleep_for(1s);
  SPDLOG_LOGGER_CRITICAL(logger, "critical message");
  EXPECT_EQ(true, true);
}
