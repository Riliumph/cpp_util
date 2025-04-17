#ifndef INCLUDE_CHRONO_ELAPSE_HPP
#define INCLUDE_CHRONO_ELAPSE_HPP
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#if defined(PRINT_IN_TESTING)
#include "string.hpp"
#include "type.hpp"
#endif

template<
  typename TimeUnit,
  typename = typename std::enable_if_t<std::is_base_of_v<
    std::chrono::duration<typename TimeUnit::rep, typename TimeUnit::period>,
    TimeUnit>>>
bool
is_unit_time_changed(std::chrono::system_clock::time_point start,
                     std::chrono::system_clock::time_point end)
{
  auto floored_start = std::chrono::floor<TimeUnit>(start);
  auto floored_end = std::chrono::floor<TimeUnit>(end);
#if defined(PRINT_IN_TESTING)
  std::cout << "start: " << to_iso8601<TimeUnit>(floored_start) << std::endl;
  std::cout << "end: " << to_iso8601<TimeUnit>(floored_end) << std::endl;
#endif
  return floored_start < floored_end;
}
#endif // INCLUDE_CHRONO_ELAPSE_HPP
