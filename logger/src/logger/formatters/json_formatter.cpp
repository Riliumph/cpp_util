#include "json_formatter.hpp"

namespace logger {
namespace formatters {

/// @brief ログ成型関数
/// @param msg ログ出力の文字列
/// @param dest 成型後の文字列を格納するバッファ
void
JsonFormatter::format(const spdlog::details::log_msg& msg,
                      spdlog::memory_buf_t& dest)
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
std::unique_ptr<spdlog::formatter>
JsonFormatter::clone() const
{
  return std::make_unique<JsonFormatter>();
}

} // namespace formatters
} // namespace logger
