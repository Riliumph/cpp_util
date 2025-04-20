#include "app_logger.hpp"
// STL
#include <iostream>
// logger
#include "common.hpp"
namespace logger {
/// @brief アプリケーションロガーの名前
/// @details spdlogのグローバル空間からこの名前で取り出す
const char* const AppLogger::name = "app_logger";
/// @brief デフォルトの最大ファイルサイズ(10MB)
const size_t AppLogger::default_max_file_size = 1024 * 1024 * 10;
/// @brief デフォルトの最大ファイル数(3)
/// @details この数以上のファイル数になると古いファイルから削除される。
const size_t AppLogger::default_max_files = 3;

/// @brief コンストラクタ
/// @param filename ログファイル名
AppLogger::AppLogger(const std::string& filename)
  : logger_name_(name)
  , file_sink_(std::make_shared<file_sink_t>(filename, true))
  , console_sink_(std::make_shared<console_sink_t>())
{
  Init();
  Config();
}

/// @brief コンストラクタ
/// @param filename ログファイル名
/// @param max_file_size 最大ファイルサイズ
/// @param max_files 最大ファイル数
AppLogger::AppLogger(const std::string& filename,
                     size_t max_file_size,
                     size_t max_files)
  : logger_name_(name)
  , file_sink_(std::make_shared<file_sink_t>(filename, true))
  , console_sink_(std::make_shared<console_sink_t>())
{
  Init();
  Config();
}

/// @brief ロガーの初期化処理
void
AppLogger::Init()
{
  // ADLの関係かInitializer-listが正しく暗黙的変換されないので型を明示する
  spdlog::sinks_init_list sinks = { console_sink_, file_sink_ };
  logger_ = std::make_shared<spdlog::logger>(logger_name_, sinks);
  spdlog::register_logger(logger_);
}

/// @brief デストラクタ
/// @details ロガーの登録解除とsinkのflushを行う
AppLogger::~AppLogger()
{
  spdlog::drop(logger_name_);
  file_sink_->flush();
  console_sink_->flush();
}

/// @brief ロガーの設定を行う
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

/// @brief ロガーにログレベルを設定する
/// @details デフォルトinfoレベルで初期化
/// @param log_level_str ログレベルの文字列
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
