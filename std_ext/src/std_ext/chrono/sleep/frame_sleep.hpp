#ifndef INCLUDED_STD_EXT_CHRONO_SLEEP_FRAME_SLEEP_HPP
#define INCLUDED_STD_EXT_CHRONO_SLEEP_FRAME_SLEEP_HPP
// STL
#include <functional>
#include <thread>
// std_ext
#include "std_ext/chrono/core/core.hpp"
#include "std_ext/chrono/math/math.hpp"

namespace std_ext {
namespace chrono {
/// @brief Hz駆動して余剰時間を待機する関数
/// @tparam TimeUnit 採用する時間単位
/// @tparam Hz 秒間の駆動回数を指定する
/// @param start 開始時刻
/// @param end 終了時刻
/// @return 待機した時間
template<
  uint32_t Hz,
  typename TimeUnit,
  typename = typename std_ext::chrono::enable_if_chrono_duration_t<TimeUnit>>
TimeUnit
sleep(std::chrono::system_clock::time_point start,
      std::chrono::system_clock::time_point end)
{
  constexpr auto TPP = time_per_period<Hz, TimeUnit>();
  auto elapsed_time = std::chrono::duration_cast<TimeUnit>(end - start);
  auto sleep_time = TimeUnit(TPP - elapsed_time.count());
  std::this_thread::sleep_for(sleep_time);
  return sleep_time;
}

/// @brief sleepのヘルパーメソッド
/// @tparam TimeUnit 採用する時間単位
/// @tparam Hz 秒間の駆動回数を指定する
/// @param fn 実行する関数
/// @return 待機した時間
template<
  uint32_t Hz,
  typename TimeUnit,
  typename = typename std_ext::chrono::enable_if_chrono_duration_t<TimeUnit>>
TimeUnit
drive_and_sleep(std::function<void()> fn)
{
  auto start = std::chrono::system_clock::now();
  fn();
  auto end = std::chrono::system_clock::now();
  auto sleep_time = sleep<Hz, TimeUnit>(start, end);
  return sleep_time;
}
} // namespace chrono
} // namespace std_ext
#endif // INCLUDED_STD_EXT_CHRONO_SLEEP_FRAME_SLEEP_HPP
