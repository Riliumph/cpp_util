#include "sleep.hpp"
// STL
#include <cmath>
#include <ctime>
#include <iostream>
#include <thread>
// original
#include "name.hpp"
#include "operator.hpp"

using namespace std::literals::chrono_literals;

/// @brief hh:mm:nのタイミングでスリープを解除する関数
/// こんな無駄なスリープ機能意味あるのか？
/// 端的に言って、Linuxのcronを使え。
/// そっちの方が長年の安全性があり、
/// @param n 時計の時刻の秒数
/// @return 待機した秒数
std::chrono::seconds
cron_sleep(double n)
{
  auto now = time(NULL);                        // 現在時刻を取得
  auto local = localtime(&now);                 // 地方時に変換
  auto quotient = std::ceil(local->tm_sec / n); // 切り上げるが型は高精度を保つ
  auto next = n * quotient;
  auto diff = static_cast<int>(next - local->tm_sec);
  std::printf("quot: %f(=%d/%f)\n", quotient, local->tm_sec, n);
  std::printf("now : %d sec\n", local->tm_sec);
  std::printf("next: %f sec(=%f*%f)\n", next, n, quotient);
  std::printf("diff: %d sec(=%f-%d)\n", diff, next, local->tm_sec);
  auto sleep_time = secs(diff);
  std::this_thread::sleep_for(sleep_time);
  std::cout << std::chrono::system_clock::now();
  return sleep_time;
}
