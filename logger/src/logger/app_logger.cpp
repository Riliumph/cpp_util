#include "app_logger.hpp"
// STL
#include <iostream>
// logger
#include "common.hpp"
namespace logger {
const char* const AppLogger::name = "app_logger";
const size_t AppLogger::default_max_file_size = 1024 * 1024 * 10; // 10MB
const size_t AppLogger::default_max_files = 3; // 保持するファイル数

AppLogger::AppLogger(const std::string& filename)
  : logger_name_(name)
  , file_sink_(std::make_shared<file_sink_t>(filename,
                                             default_max_file_size,
                                             default_max_files,
                                             true))
  , console_sink_(std::make_shared<console_sink_t>())
{
  Init();
  Config();
}

AppLogger::AppLogger(const std::string& filename,
                     size_t max_file_size,
                     size_t max_files)
  : logger_name_(name)
  , file_sink_(
      std::make_shared<file_sink_t>(filename, max_file_size, max_files, true))
  , console_sink_(std::make_shared<console_sink_t>())
{
  Init();
  Config();
}

void
AppLogger::Init()
{
  // ADLの関係かInitializer-listが正しく暗黙的変換されないので型を明示する
  spdlog::sinks_init_list sinks = { console_sink_, file_sink_ };
  logger_ = std::make_shared<spdlog::logger>(logger_name_, sinks);
  spdlog::register_logger(logger_);
}

AppLogger::~AppLogger()
{
  spdlog::drop(logger_name_);
  file_sink_->flush();
  file_sink_.reset();
  console_sink_->flush();
  console_sink_.reset();
  logger_.reset();
}

void
AppLogger::Config()
{
  if (logger_ == nullptr) {
    std::cerr << "Logger is not initialized." << std::endl;
    return;
  }
  // ログレベル設定
  auto log_level = get_env_or("LOG_LEVEL", "info");
  SetLevel(log_level);

  // フォーマッタ設定
  logger_->set_formatter(std::make_unique<formatter_t>());
}

/// @brief デフォルトinfoレベルで初期化
/// @param log_level_str
void
AppLogger::SetLevel(const std::string& log_level_str)
{
  if (logger_ == nullptr) {
    std::cerr << "Logger is not initialized." << std::endl;
    return;
  }
  auto log_level = spdlog::level::from_str(log_level_str);
  if (log_level == spdlog::level::off) {
    std::cerr << "Invalid log level: " << log_level_str << std::endl;
    std::cerr << "set default log level" << std::endl;
    log_level = spdlog::level::info;
  }
  logger_->set_level(log_level);
}

}
