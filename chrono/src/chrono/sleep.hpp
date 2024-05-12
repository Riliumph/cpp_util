#ifndef INCLUDE_CHRONO_WAIT_HPP
#define INCLUDE_CHRONO_WAIT_HPP
// STL
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
  std::this_thread::sleep_for(sleep_time);
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
  auto frame_time = std::chrono::duration<double>(1.0 / FPS);
  auto start = std::chrono::system_clock::now();
  fn();
  auto end = std::chrono::system_clock::now();
  auto elapsed_time = std::chrono::duration_cast<TimeUnit>(end - start);
  auto sleep_time = frame_time - elapsed_time;
  if (0 < sleep_time.count()) {
    std::this_thread::sleep_for(sleep_time);
  } else {
    std::cerr << "over time(" << sleep_time.count() << ")" << std::endl;
  }
  return sleep_time;
}

#endif // INCLUDE_CHRONO_WAIT_HPP
