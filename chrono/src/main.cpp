// STL
#include <chrono>
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
  return 0;
}
