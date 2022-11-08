#include <chrono>
#include <iostream>
#include <time.h>

using namespace std::literals::chrono_literals;

int
main()
{
  std::cout << "current time" << std::endl;
  auto now = std::chrono::system_clock::now();
  auto t = std::chrono::system_clock::to_time_t(now);
  std::cout << ctime(&t) << std::endl;

  std::cout << "current time with ceiled sec" << std::endl;
  auto ceiled_now = std::chrono::ceil<std::chrono::minutes>(now);
  auto ceiled_t = std::chrono::system_clock::to_time_t(ceiled_now);
  std::cout << ctime(&ceiled_t) << std::endl;

  std::cout << "current time with floored sec" << std::endl;
  auto floored_now = std::chrono::floor<std::chrono::minutes>(now);
  auto floored_t = std::chrono::system_clock::to_time_t(floored_now);
  std::cout << ctime(&floored_t) << std::endl;

  std::cout << "1min future time" << std::endl;
  auto now_1m = now + 1min;
  auto t_1m = std::chrono::system_clock::to_time_t(now_1m);
  std::cout << ctime(&t_1m) << std::endl;
  return 0;
}
