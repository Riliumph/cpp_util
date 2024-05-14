#ifndef INCLUDE_CHRONO_WAIT_HPP
#define INCLUDE_CHRONO_WAIT_HPP
// STL
#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <type_traits>
// original
#include "operator.hpp"

std::chrono::seconds
cron_sleep(double);

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
  constexpr auto one_sec = std::chrono::seconds(1);
  constexpr auto unit_time_in_sec =
    std::chrono::duration_cast<TimeUnit>(one_sec);
  constexpr int unit_time_in_frame = unit_time_in_sec.count() / FPS;
  auto elapsed_time = std::chrono::duration_cast<TimeUnit>(end - start);
  auto sleep_time = TimeUnit(unit_time_in_frame - elapsed_time.count());
  if (0 < sleep_time.count()) {
    std::this_thread::sleep_for(sleep_time);
  }
  return sleep_time;
}

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
  TimeUnit sleep_time(std::max(0L, drive_time.count() - elapsed_time.count()));
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
#endif // INCLUDE_CHRONO_WAIT_HPP
