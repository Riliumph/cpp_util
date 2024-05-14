#ifndef INCLUDE_CHRONO_FRAME_FRAME_HPP
#define INCLUDE_CHRONO_FRAME_FRAME_HPP
#include <chrono>

template<uint32_t FPS, typename TimeUnit>
constexpr int
time_per_frame()
{
  auto one_sec = std::chrono::seconds(1);
  auto unit_time_in_sec = std::chrono::duration_cast<TimeUnit>(one_sec);
  int unit_time_in_frame = unit_time_in_sec.count() / FPS;
  return unit_time_in_frame;
}

#endif // INCLUDE_CHRONO_FRAME_FRAME_HPP
