#ifndef INCLUDE_CHRONO_FRAME_FRAME_HPP
#define INCLUDE_CHRONO_FRAME_FRAME_HPP

template<typename UnitTime>
int
FrameCountInSec()
{
  constexpr auto one_sec = std::chrono::seconds(1);
  auto one_sec_duration = std::chrono::duration_cast<UnitTime>(one_second);
  int frames_in_sec = one_second_duration / UnitTime(1);
  return frames_in_sec;
}

#endif // INCLUDE_CHRONO_FRAME_FRAME_HPP
