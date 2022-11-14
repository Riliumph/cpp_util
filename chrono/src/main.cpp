#include <chrono>
#include <cmath>
#include <iostream>
#include <ostream>
#include <thread>
#include <time.h>

using namespace std::literals::chrono_literals;
using namespace std::chrono;

void
scheduled_task(double);

std::ostream&
operator<<(std::ostream& os, std::chrono::system_clock::time_point tp)
{
  auto t = std::chrono::system_clock::to_time_t(tp);
  os << ctime(&t);
  return os;
}

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

  //　秒フィールドの取得(C++17)

  // 時計の秒単位で動作する
  scheduled_task(5.0);
  return 0;
}

/// @brief n秒の度に動く
/// @param n 時計の時刻の秒数
void
scheduled_task(double n)
{
  auto now = time(NULL);                        // 現在時刻を取得
  auto local = localtime(&now);                 // 地方時に変換
  auto quotient = std::ceil(local->tm_sec / n); //切り上げるが型は高精度を保つ
  auto next = n * quotient;
  auto diff = static_cast<int>(next - local->tm_sec);
  std::printf("quot: %f(=%d/%f)\n", quotient, local->tm_sec, n);
  std::printf("now : %d sec\n", local->tm_sec);
  std::printf("next: %f sec(=%f*%f)\n", next, n, quotient);
  std::printf("diff: %d sec(=%f-%d)\n", diff, next, local->tm_sec);
  std::this_thread::sleep_for(std::chrono::seconds(diff));
  std::cout << std::chrono::system_clock::now();
}
