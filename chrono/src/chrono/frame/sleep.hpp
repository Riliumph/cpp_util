#ifndef INCLUDE_CHRONO_FRAME_SLEEP_HPP
#define INCLUDE_CHRONO_FRAME_SLEEP_HPP
// STL
#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <type_traits>
// original
#include "chrono/frame/frame.hpp"
#include "chrono/operator_io.hpp"

/// @brief FPSを計算して余剰時間を待機する関数
/// @tparam TimeUnit 採用する時間単位
/// @tparam FPS FPSの数値
/// @param start 開始時刻
/// @param end 終了時刻
/// @return 待機した時間
template<
  uint32_t FPS,
  typename TimeUnit,
  typename = typename std::enable_if<std::is_base_of<
    std::chrono::duration<typename TimeUnit::rep, typename TimeUnit::period>,
    TimeUnit>::value>::type>
TimeUnit
fps_sleep(std::chrono::system_clock::time_point start,
          std::chrono::system_clock::time_point end)
{
  constexpr auto TPF = time_per_frame<FPS, TimeUnit>();
  auto elapsed_time = std::chrono::duration_cast<TimeUnit>(end - start);
  auto sleep_time = TimeUnit(TPF - elapsed_time.count());
  std::this_thread::sleep_for(sleep_time);
  return sleep_time;
}

/// @brief fps_sleepのヘルパーメソッド（オーバーロード定義）
/// @tparam TimeUnit 採用する時間単位
/// @tparam FPS FPSの数値
/// @param fn 実行する関数
/// @return 待機した時間
template<
  uint32_t FPS,
  typename TimeUnit,
  typename = typename std::enable_if<std::is_base_of<
    std::chrono::duration<typename TimeUnit::rep, typename TimeUnit::period>,
    TimeUnit>::value>::type>
TimeUnit
fps_sleep(std::function<void()> fn)
{
  auto start = std::chrono::system_clock::now();
  fn();
  auto end = std::chrono::system_clock::now();
  auto sleep_time = fps_sleep<FPS, TimeUnit>(start, end);
  return sleep_time;
}
#endif // INCLUDE_CHRONO_FRAME_SLEEP_HPP
