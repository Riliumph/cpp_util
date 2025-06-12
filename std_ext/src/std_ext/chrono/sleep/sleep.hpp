#ifndef INCLUDED_STD_EXT_CHRONO_CHRONO_SLEEP_SLEEP_HPP
#define INCLUDED_STD_EXT_CHRONO_CHRONO_SLEEP_SLEEP_HPP
// STL
#include <chrono>
#include <functional>
#include <thread>
// std_ext
#include "std_ext/chrono/core/core.hpp"

namespace std_ext {
namespace chrono {
std::chrono::seconds
cron_sleep(double);

/// @brief 指定した周期の余剰時間を待機する関数
/// @tparam TimeUnit 採用する時間単位
/// @param drive_time 周期時間
/// @param start 開始時刻
/// @param end 終了時刻
/// @return 待機した時間
template<
  typename TimeUnit,
  typename = typename std_ext::chrono::enable_if_chrono_duration_t<TimeUnit>>
TimeUnit
flexible_sleep(TimeUnit drive_time,
               std::chrono::system_clock::time_point start,
               std::chrono::system_clock::time_point end)
{
  auto elapsed_time = std::chrono::duration_cast<TimeUnit>(end - start);
  auto sleep_time = TimeUnit(drive_time - elapsed_time);
  std::this_thread::sleep_for(sleep_time);
  return sleep_time;
}

/// @brief dynamic_sleepのヘルパーメソッド
/// @tparam TimeUnit 採用した時間単位
/// @param drive_time 周期時間
/// @param fn 実行する関数
/// @return 待機した時間
template<
  typename TimeUnit,
  typename = typename std_ext::chrono::enable_if_chrono_duration_t<TimeUnit>>
TimeUnit
flexible_sleep(TimeUnit drive_time, std::function<void()> fn)
{
  auto start = std::chrono::system_clock::now();
  fn();
  auto end = std::chrono::system_clock::now();
  auto sleep_time = flexible_sleep(drive_time, start, end);
  return sleep_time;
}
} // namespace chrono
} // namespace std_ext
#endif // INCLUDED_STD_EXT_CHRONO_CHRONO_SLEEP_SLEEP_HPP
