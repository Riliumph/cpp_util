#ifndef INCLUDE_CHRONO_CALC_HPP
#define INCLUDE_CHRONO_CALC_HPP
#include <chrono>
#include <cmath>

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

#endif // INCLUDE_CHRONO_CALC_HPP
