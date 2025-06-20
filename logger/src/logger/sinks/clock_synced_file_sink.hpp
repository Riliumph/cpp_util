#ifndef INCLUDE_LOGGER_SINKS_CLOCK_SYNCED_FILE_SINK_HPP
#define INCLUDE_LOGGER_SINKS_CLOCK_SYNCED_FILE_SINK_HPP
// STL
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
// spdlog
#include <spdlog/details/file_helper.h>
#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>
// original
#include <std_ext/std_ext.hpp>

namespace logger {
namespace sinks {
namespace ext = std_ext;
/// @brief タイムスタンプを付与するローテーションファイルシンク
/// @tparam Mutex ミューテックスの型
/// @details ログファイル名にタイムスタンプを付与してローテーションする
template<typename Mutex,
         typename TimeUnit,
         typename = typename ext::chrono::is_chrono_duration<TimeUnit>>
class clock_synced_file_sink final : public spdlog::sinks::base_sink<Mutex>
{
public:
  using super_sink = spdlog::sinks::base_sink<Mutex>;
  using clock_t = spdlog::log_clock;
  using time_point_t = clock_t::time_point;
  using time_unit_on_filename = std::chrono::seconds;
  using rotation_cycle_t = TimeUnit;

public:
  /// @brief クラス名
  static constexpr const char* const class_name = "clock_synced_file_sink";

public:
  /// @brief コンストラクタ
  /// @param log_filename ログファイル名
  /// @param rotate_on_open 最初に開いたときにローテーションするか
  /// @param event_handlers イベントハンドラ
  explicit clock_synced_file_sink(
    spdlog::filename_t log_filename,
    const spdlog::file_event_handlers& event_handlers = {})
    : base_filename_(log_filename)
    , file_helper_{ event_handlers }
  {
    file_helper_.open(base_filename_);
    opened_at_ = clock_t::now();
  }

  /// @brief デストラクタ
  /// @details ログファイルを閉じて、ローテーションする
  ~clock_synced_file_sink()
  {
    flush_();
    file_helper_.close();
    move_to_rotate(clock_t::now());
    // not open new file: file_helper_.reopen(true);
  }

  /// @brief ファイル名を決める関数
  /// @param filename ベースとなるファイル名
  /// @param timestamp 付与したいタイムスタンプ
  /// @return ファイル名
  static spdlog::filename_t calc_filename(const spdlog::filename_t& filename,
                                          time_point_t timestamp)
  {
    using fh = spdlog::details::file_helper;
    auto [basename, extension] = fh::split_by_extension(filename);
    auto ts = ext::chrono::to_iso8601<time_unit_on_filename>(timestamp);
    std::replace(ts.begin(), ts.end(), ':', '-');
    return fmt::format("{}_{}{}", basename, ts, extension);
  }

protected:
  /// @brief ログメッセージをファイルに書き込む関数
  /// @details logger->infoなどの関数で自動で使用される。
  /// @param msg ログメッセージ
  void sink_it_(const spdlog::details::log_msg& msg) override
  {
    if (should_rotate_(clock_t::now())) {
      rotate_();
    }
    spdlog::memory_buf_t formatted;
    super_sink::formatter_->format(msg, formatted);
    file_helper_.write(formatted);
  }

  /// @brief ログメッセージをファイルに書き込む関数
  /// @details spdlogによって自動で使用される。
  void flush_() override { file_helper_.flush(); }

private:
  /// @brief ファイルローテーションを行う関数
  /// @details ローテーションは、閉じる、退避、新規作成の３ステップで行う
  void rotate_()
  {
    // close file
    file_helper_.close();
    // move file
    auto will_rotate_at = clock_t::now();
    auto rotated = move_to_rotate(will_rotate_at);
    // reopen file
    if (rotated) {
      auto rotated_at = will_rotate_at;
      file_helper_.reopen(true);
      opened_at_ = rotated_at;
    }
  }

  /// @brief ファイルを移動させる関数
  /// @param will_rotate_at ローテーション予定の時刻
  /// @return ローテーションの成否
  bool move_to_rotate(time_point_t will_rotate_at)
  {
    auto src = file_helper_.filename();
    auto dst = calc_filename(base_filename_, will_rotate_at);
    return spdlog::details::os::rename(src, dst) == 0;
  }

  /// @brief ローテーションするべき時間かどうかを判断する関数
  /// @details 前回開いた時刻からTimeUnit分過ぎたかどうかを判定する。
  /// @param will_rotate_at ローテーション予定の時刻
  /// @return ローテーションすべきかどうか
  bool should_rotate_(const time_point_t& will_rotate_at) const
  {
    return ext::chrono::is_unit_time_changed<TimeUnit>(opened_at_,
                                                       will_rotate_at);
  }

private:
  spdlog::filename_t base_filename_;
  time_point_t opened_at_;
  spdlog::details::file_helper file_helper_;
};

/// @brief マルチスレッド用の時刻同期ローテーションファイルシンク
/// @tparam TimeUnit 時刻の単位
template<typename TimeUnit>
using clock_synced_file_sink_mt = clock_synced_file_sink<std::mutex, TimeUnit>;
/// @brief シングルスレッド用の時刻同期ローテーションファイルシンク
/// @tparam TimeUnit 時刻の単位
template<typename TimeUnit>
using clock_synced_file_sink_st =
  clock_synced_file_sink<spdlog::details::null_mutex, TimeUnit>;
} // namespace sinks
} // namespace logger
#endif // INCLUDE_LOGGER_SINKS_CLOCK_SYNCED_FILE_SINK_HPP
