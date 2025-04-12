#include "logger.hpp"
// STL
#include <iostream>
// logger
#include "common.hpp"
#include "json_formatter.hpp"
namespace logger {
const char* Logger::name = "app_logger";

Logger::Logger(const std::string& filename)
  : logger_name_(name)
  , file_sink_(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      filename,
      1024 * 1024 * 10, // 10MB
      3,                // ローテーション数
      true))            // append
  , console_sink_(std::make_shared<spdlog::sinks::stdout_color_sink_mt>())
{
  // ADLの関係かInitializer-listが正しく暗黙的変換されないので型を明示する
  spdlog::sinks_init_list sinks = { console_sink_, file_sink_ };
  logger_ = std::make_shared<spdlog::logger>(name, sinks);
  spdlog::register_logger(logger_);
  Config();
}

Logger::~Logger()
{
  spdlog::drop(logger_name_);
  file_sink_->flush();
  file_sink_.reset();
  console_sink_->flush();
  console_sink_.reset();
  logger_.reset();
}

void
Logger::Config()
{
  if (logger_ == nullptr) {
    std::cerr << "Logger is not initialized." << std::endl;
    return;
  }
  // ログレベル設定
  auto log_level = get_env_or("LOG_LEVEL", "info");
  SetLevel(log_level);

  // フォーマッタ設定
  logger_->set_formatter(std::make_unique<JsonFormatter>());
}

/// @brief デフォルトinfoレベルで初期化
/// @param log_level_str
void
Logger::SetLevel(const std::string& log_level_str)
{
  auto log_level = spdlog::level::from_str(log_level_str);
  if (log_level == spdlog::level::off) {
    std::cerr << "Invalid log level: " << log_level_str << std::endl;
    std::cerr << "set default log level" << std::endl;
    log_level = spdlog::level::info;
  }
  logger_->set_level(log_level);
}

}
