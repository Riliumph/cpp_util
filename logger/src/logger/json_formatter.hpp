#ifndef INCLUDE_LOGGER_JSON_FORMATTER_HPP
#define INCLUDE_LOGGER_JSON_FORMATTER_HPP
// STL
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
// json
#include <nlohmann/json.hpp>
// spdlog
#include <spdlog/details/log_msg.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
// fmt
#include <fmt/format.h>
namespace logger {
/// @brief JSONフォーマッタ
/// @details spdlogのフォーマッタを継承してJSON形式で出力する
class JsonFormatter : public spdlog::formatter
{
public:
  /// @brief ISO8601形式のタイムスタンプフォーマット
  static constexpr const char* const iso8601 = "%FT%T%z";

public:
  /// @brief ログ成型関数
  /// @param msg ログ出力の文字列
  /// @param dest 成型後の文字列を格納するバッファ
  void format(const spdlog::details::log_msg& msg,
              spdlog::memory_buf_t& dest) override
  {
    nlohmann::ordered_json j;
    { // timestamp
      std::ostringstream ts;
      std::time_t t = std::chrono::system_clock::to_time_t(msg.time);
      ts << std::put_time(std::localtime(&t), iso8601);
      j["timestamp"] = ts.str();
    }
    { // logger name
      j["logger"] = msg.logger_name.data();
    }
    { // thread id
      j["tid"] = msg.thread_id;
    }
    { // source location
      // spdlog::source_locが渡されてない場合は出力しない
      if (!msg.source.empty()) {
        j["file"] = msg.source.filename;
        j["line"] = msg.source.line;
        j["func"] = msg.source.funcname;
      }
    }
    { // log level
      j["level"] = spdlog::level::to_string_view(msg.level).data();
    }
    { // message
#if defined(EXPERIMENTAL_NESTED_JSON)
      { // payload に JSON 拡張部分が含まれていればパースしてマージ
        try {
          auto ext = nlohmann::json::parse(msg.payload.data(), nullptr, false);
          if (ext.is_object()) {
            j.update(ext);
          }
        } catch (...) {
          // 無視してOK
        }
      }
#else
      j["message"] = fmt::to_string(msg.payload);
#endif
    }
    fmt::format_to(std::back_insert_iterator(dest), "{}\n", j.dump());
  }

  /// @brief フォーマッタのクローンを作成する
  std::unique_ptr<spdlog::formatter> clone() const override
  {
    return std::make_unique<JsonFormatter>();
  }
};
}
#endif // INCLUDE_LOGGER_JSON_FORMATTER_HPP
