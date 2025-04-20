#ifndef INCLUDE_LOGGER_SINKS_TIMELY_ROTATING_FILE_SINK_HPP
#define INCLUDE_LOGGER_SINKS_TIMELY_ROTATING_FILE_SINK_HPP
// STL
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
#include <chrono.hpp>

namespace logger {
namespace sinks {

/// @brief タイムスタンプを付与するローテーションファイルシンク
/// @tparam Mutex ミューテックスの型
/// @details ログファイル名にタイムスタンプを付与してローテーションする
template<
  typename Mutex,
  typename TimeUnit,
  typename = typename std::enable_if_t<std::is_base_of_v<
    std::chrono::duration<typename TimeUnit::rep, typename TimeUnit::period>,
    TimeUnit>>>
class timely_rotating_file_sink final : public spdlog::sinks::base_sink<Mutex>
{
public:
  using super_sink = spdlog::sinks::base_sink<Mutex>;
  using clock_t = spdlog::log_clock;
  using time_point_t = clock_t::time_point;
  using time_unit_on_filename = std::chrono::seconds;
  using rotation_cycle_t = TimeUnit;

public:
  /// @brief クラス名
  static constexpr const char* const class_name = "timely_rotating_file_sink";

public:
  /// @brief コンストラクタ
  /// @param log_filename ログファイル名
  /// @param rotate_on_open 最初に開いたときにローテーションするか
  /// @param event_handlers イベントハンドラ
  explicit timely_rotating_file_sink(
    spdlog::filename_t log_filename,
    bool rotate_on_open = false,
    const spdlog::file_event_handlers& event_handlers = {})
    : base_filename_(log_filename)
    , file_helper_{ event_handlers }
  {
    file_helper_.open(base_filename_);
    opened_at_ = clock_t::now();
  }

  /// @brief ファイル名を決める関数
  /// @param filename ベースとなるファイル名
  /// @param timestamp 付与したいタイムスタンプ
  /// @return ファイル名
  static spdlog::filename_t calc_filename(const spdlog::filename_t& filename,
                                          time_point_t timestamp)
  {
    using fh = spdlog::details::file_helper;
    auto [basename, ext] = fh::split_by_extension(filename);
    auto ts = to_iso8601<time_unit_on_filename>(timestamp);
    return fmt::format("{}_{}{}", basename, ts, ext);
  }

protected:
  /// @brief ログメッセージをファイルに書き込む関数
  /// @details logger->infoなどの関数で自動で使用される。
  /// @param msg ログメッセージ
  void sink_it_(const spdlog::details::log_msg& msg) override
  {
    if (should_rotate(clock_t::now())) {
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
  void rotate_()
  {
    file_helper_.close();
    auto will_rotate_at = clock_t::now();
    auto cur_filename = file_helper_.filename();
    auto new_filename = calc_filename(base_filename_, will_rotate_at);
    // TODO: 成功前提とする
    rename_file_(cur_filename, new_filename);
    auto rotated_at = will_rotate_at;
    file_helper_.reopen(true);
    opened_at_ = rotated_at;
  }

  /// @brief ファイル名を変更する関数
  /// @param src_filename 移動元のファイル名
  /// @param dst_filename 移動先のファイル名
  /// @return 成否
  bool rename_file_(const spdlog::filename_t& src_filename,
                    const spdlog::filename_t& dst_filename)
  {
    // caution:
    // new_filenameがすでに存在する場合、上書きされるのでファイルは失われる。
    return spdlog::details::os::rename(src_filename, dst_filename) == 0;
  }

  /// @brief ローテーションするべき時間かどうかを計算する関数
  /// @details
  /// 前回ローテーションした時刻からTimeUnit分過ぎたかどうかを判定する。
  /// @param tp 対象の時間
  /// @return ローテーションすべきかどうか
  bool should_rotate(const time_point_t& tp) const
  {
    return is_unit_time_changed<TimeUnit>(opened_at_, tp);
  }

private:
  spdlog::filename_t base_filename_;
  time_point_t opened_at_;
  spdlog::details::file_helper file_helper_;
};

/// @brief マルチスレッド用のタイムスタンプローテーションファイルシンク
template<typename T>
using timely_rotating_file_sink_mt = timely_rotating_file_sink<std::mutex, T>;
/// @brief シングルスレッド用のタイムスタンプローテーションファイルシンク
template<typename T>
using timely_rotating_file_sink_st =
  timely_rotating_file_sink<spdlog::details::null_mutex, T>;
} // namespace sinks
} // namespace logger
#endif // INCLUDE_LOGGER_SINKS_TIMELY_ROTATING_FILE_SINK_HPP
