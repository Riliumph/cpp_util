#ifndef INCLUDED_STD_EXT_CHRONO_MATH_MATH_HPP
#define INCLUDED_STD_EXT_CHRONO_MATH_MATH_HPP
#include <cmath>
// std_ext
#include "std_ext/chrono/core/core.hpp"

namespace std_ext {
namespace chrono {
/// @brief 単位時間の桁数を計算する関数
/// @tparam TimeUnit 単位時間
/// @return 単位時間の桁数
template<
  typename TimeUnit,
  typename = typename std::enable_if_t<std::is_base_of_v<
    std::chrono::duration<typename TimeUnit::rep, typename TimeUnit::period>,
    TimeUnit>>>
int
calc_digit()
{
  constexpr auto denom = TimeUnit::period::den;
  constexpr auto num = TimeUnit::period::num;

  // 秒より細かい単位（分母が1より大きい）だけ処理
  if constexpr (denom <= num) {
    return 0;
  }

  return static_cast<int>(
    std::ceil(std::log10(denom / static_cast<double>(num))));
}

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

/// @brief 任意の時間単位が変化したかどうかを判定する
/// @details 分が設定された場合、00:00:00から00:01:00になったときにtrueが返る
/// @tparam TimeUnit 任意の時間単位
/// @param start 開始時刻
/// @param end 終了時刻
/// @return 変化の有無
template<
  typename TimeUnit,
  typename = typename std::enable_if_t<std::is_base_of_v<
    std::chrono::duration<typename TimeUnit::rep, typename TimeUnit::period>,
    TimeUnit>>>
bool
is_unit_time_changed(std::chrono::system_clock::time_point start,
                     std::chrono::system_clock::time_point end)
{
  auto floored_start = std::chrono::floor<TimeUnit>(start);
  auto floored_end = std::chrono::floor<TimeUnit>(end);
#if defined(PRINT_IN_TESTING)
  std::cout << "start: " << to_iso8601<TimeUnit>(floored_start) << std::endl;
  std::cout << "end: " << to_iso8601<TimeUnit>(floored_end) << std::endl;
#endif
  return floored_start < floored_end;
}
} // namespace chrono
} // namespace std_ext
#endif // INCLUDED_STD_EXT_CHRONO_MATH_MATH_HPP
