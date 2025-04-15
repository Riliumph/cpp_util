#ifndef INCLUDE_LOGGER_DEFINE_HPP
#define INCLUDE_LOGGER_DEFINE_HPP
#include "app_logger.hpp"
// spdlog
#include <spdlog/spdlog.h>
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
#endif // INCLUDE_LOGGER_DEFINE_HPP
