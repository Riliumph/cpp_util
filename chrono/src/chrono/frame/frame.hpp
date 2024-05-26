#ifndef INCLUDE_CHRONO_FRAME_FRAME_HPP
#define INCLUDE_CHRONO_FRAME_FRAME_HPP
#include <chrono>

/// @brief 周波数から1周期にかかる時間を求める関数
/// @tparam Hz 秒間の駆動数を指定する
/// @tparam TimeUnit 演算粒度
/// @return
template<uint32_t Hz, typename TimeUnit>
constexpr int
time_per_period()
{
  auto one_sec = std::chrono::seconds(1);
  // 単位変換（e.g. 1 sec -> 1,000 msec）
  auto n_unit_time = std::chrono::duration_cast<TimeUnit>(one_sec);
  // 1周期の時間を算出
  int t = n_unit_time.count() / Hz;
  return t;
}

#endif // INCLUDE_CHRONO_FRAME_FRAME_HPP
