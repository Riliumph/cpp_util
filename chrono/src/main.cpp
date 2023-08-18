// STL
#include <cmath>
#include <ctime>
#include <iostream>
#include <thread>
// original
#include "chrono.hpp"

using namespace std::literals::chrono_literals;
using minutes = std::chrono::minutes;

void
scheduled_task(double);

int
main()
{
  std::cout << "current time" << std::endl;
  auto now = std::chrono::system_clock::now();
  std::cout << now;

  std::cout << "current time with ceiled sec" << std::endl;
  auto ceiled_now = std::chrono::ceil<minutes>(now);
  std::cout << ceiled_now;

  std::cout << "current time with floored sec" << std::endl;
  auto floored_now = std::chrono::floor<minutes>(now);
  std::cout << floored_now;

  std::cout << "1min future time" << std::endl;
  auto now_1m = now + 1min;
  std::cout << now_1m;

  // 　秒フィールドの取得(C++17)

  // 時計の秒単位で動作する
  scheduled_task(5.0);
  return 0;
}

/// @brief hh:mm:nのタイミングでスリープを解除する関数
/// こんな無駄なスリープ機能意味あるのか？
/// 端的に言って、Linuxのcronを使え。
/// そっちの方が長年の安全性があり、
/// @param n 時計の時刻の秒数
void
scheduled_task(double n)
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
  std::this_thread::sleep_for(std::chrono::seconds(diff));
  std::cout << std::chrono::system_clock::now();
}
