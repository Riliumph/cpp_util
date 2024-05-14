#ifndef INCLUDE_CHRONO_SLEEP_HPP
#define INCLUDE_CHRONO_SLEEP_HPP
// STL
#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <type_traits>
// original
#include "operator_io.hpp"

std::chrono::seconds
cron_sleep(double);

template<
  typename TimeUnit,
  typename = typename std::enable_if<std::is_base_of<
    std::chrono::duration<typename TimeUnit::rep, typename TimeUnit::period>,
    TimeUnit>::value>::type>
TimeUnit
dynamic_sleep(TimeUnit drive_time,
              std::chrono::system_clock::time_point start,
              std::chrono::system_clock::time_point end)
{
  auto elapsed_time = std::chrono::duration_cast<TimeUnit>(end - start);
  auto sleep_time = TimeUnit(drive_time - elapsed_time);
  std::this_thread::sleep_for(sleep_time);
  return sleep_time;
}

template<
  typename TimeUnit,
  typename = typename std::enable_if<std::is_base_of<
    std::chrono::duration<typename TimeUnit::rep, typename TimeUnit::period>,
    TimeUnit>::value>::type>
TimeUnit
dynamic_sleep_helper(TimeUnit drive_time, std::function<void()> fn)
{
  auto start = std::chrono::system_clock::now();
  fn();
  auto end = std::chrono::system_clock::now();
  auto sleep_time = dynamic_sleep(drive_time, start, end);
  return sleep_time;
}
#endif // INCLUDE_CHRONO_SLEEP_HPP
