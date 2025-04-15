#ifndef INCLUDE_TIMESTAMP_ROTATING_FILE_SINK_HPP
#define INCLUDE_TIMESTAMP_ROTATING_FILE_SINK_HPP
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
namespace logger {
template<typename Mutex>
class timestamp_rotating_file_sink final
  : public spdlog::sinks::base_sink<Mutex>
{
public:
  static constexpr const char* const class_name =
    "timestamp_rotating_file_sink";
  static constexpr std::size_t min_file_size = 0;
  static constexpr std::size_t max_max_files = 200000;
  static constexpr const char* const time_format = "%Y-%m-%dT%H-%M-%S";

public:
  explicit timestamp_rotating_file_sink(
    spdlog::filename_t log_filename,
    std::size_t max_size,
    std::size_t max_files,
    bool rotate_on_open = false,
    const spdlog::file_event_handlers& event_handlers = {})
    : base_filename_(log_filename)
    , max_size_(max_size)
    , max_files_(max_files)
    , file_helper_{ event_handlers }
  {
    if (max_size == min_file_size) {
      auto msg = fmt::format(
        "{} constructor: max_size arg cannot be {}", class_name, min_file_size);
      spdlog::throw_spdlog_ex(msg);
    }

    if (max_max_files < max_files) {
      auto msg = fmt::format("{} constructor: max_files arg cannot exceed {}",
                             class_name,
                             max_max_files);
      spdlog::throw_spdlog_ex(msg);
    }
    file_helper_.open(base_filename_);
    current_size_ = file_helper_.size();
    if (rotate_on_open && current_size_ > 0) {
      rotate_();
      current_size_ = 0;
    }
  }

  static spdlog::filename_t calc_filename(
    const spdlog::filename_t& filename,
    std::chrono::system_clock::time_point timestamp)
  {
    auto [basename, ext] =
      spdlog::details::file_helper::split_by_extension(filename);
    std::ostringstream ts;
    auto time = std::chrono::system_clock::to_time_t(timestamp);
    ts << std::put_time(std::localtime(&time), time_format);
    return fmt::format("{}_{}{}", basename, ts.str(), ext);
  }

protected:
  void sink_it_(const spdlog::details::log_msg& msg) override
  {
    spdlog::memory_buf_t formatted;
    spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
    auto new_size = current_size_ + formatted.size();
    if (max_size_ < new_size) {
      file_helper_.flush();
      if (0 < file_helper_.size()) {
        rotate_();
        new_size = formatted.size();
      }
    }
    file_helper_.write(formatted);
    current_size_ = new_size;
  }

  void flush_() override { file_helper_.flush(); }

private:
  void rotate_()
  {
    file_helper_.close();
    auto now = std::chrono::system_clock::now();
    auto cur_filename = file_helper_.filename();
    auto new_filename = calc_filename(base_filename_, now);
    rename_file_(cur_filename, new_filename);
    file_helper_.reopen(true);
  }

  bool rename_file_(const spdlog::filename_t& src_filename,
                    const spdlog::filename_t& dst_filename)
  {
    // caution:
    // new_filenameがすでに存在する場合、上書きされるのでファイルは失われる。
    return spdlog::details::os::rename(src_filename, dst_filename) == 0;
  }

private:
  spdlog::filename_t base_filename_;
  std::size_t max_size_;
  std::size_t max_files_;
  std::size_t current_size_;
  spdlog::details::file_helper file_helper_;
};

// エイリアス（マルチスレッド版）
using timestamp_rotating_file_sink_mt =
  timestamp_rotating_file_sink<std::mutex>;
// シングルスレッドなら null_mutex で
using timestamp_rotating_file_sink_st =
  timestamp_rotating_file_sink<spdlog::details::null_mutex>;
}
#endif // INCLUDE_TIMESTAMP_ROTATING_FILE_SINK_HPP
