#ifndef INCLUDE_CHRONO_STRING_HPP
#define INCLUDE_CHRONO_STRING_HPP
// STL
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
// original
#include "calc.hpp"
#include "type.hpp"

/// @brief ISO8601形式で時間を文字列化する関数
/// @tparam TimeUnit 計算したい時間の単位（デフォルトは秒）
/// @param tp 特定の時間
/// @return ISO8601形式の文字列
template<
  typename TimeUnit = std::chrono::seconds,
  typename = typename std::enable_if_t<std::is_base_of_v<
    std::chrono::duration<typename TimeUnit::rep, typename TimeUnit::period>,
    TimeUnit>>>
std::string
to_iso8601(std::chrono::system_clock::time_point tp)
{
  static constexpr const char* const fmt = "%Y-%m-%dT%H:%M:%S";
  auto digits = calc_digit<TimeUnit>();

  // 秒数以下の数値を取得
  auto duration = tp.time_since_epoch();
  auto seconds = std::chrono::duration_cast<secs>(duration);
  auto subs = std::chrono::duration_cast<TimeUnit>(duration - seconds);
  // 文字列化処理
  auto t = std::chrono::system_clock::to_time_t(tp);
  auto tm = *std::localtime(&t);
  std::stringstream ss;
  ss << std::put_time(&tm, fmt);
  if (0 < digits) {
    ss << "." << std::setfill('0') << std::setw(digits) << subs.count();
  }
  return ss.str();
}

#endif // INCLUDE_CHRONO_STRING_HPP
