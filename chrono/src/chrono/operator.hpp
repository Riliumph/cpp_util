#ifndef INCLUDE_CHRONO_OPERATOR_HPP
#define INCLUDE_CHRONO_OPERATOR_HPP
#include <chrono>
#include <ostream>

std::ostream&
operator<<(std::ostream&, const std::chrono::system_clock::time_point&);

template<typename Rep, typename Period>
std::ostream&
operator<<(std::ostream& os, const std::chrono::duration<Rep, Period>& lv)
{
  using namespace std::chrono;
  using nsecs = std::chrono::nanoseconds;
  using usecs = std::chrono::microseconds;
  using msecs = std::chrono::milliseconds;
  using secs = std::chrono::seconds;
  using mins = std::chrono::minutes;
  using hours = std::chrono::hours;

  if constexpr (std::ratio_equal_v<Period, nsecs::period>) {
    os << lv.count() << "ns";
  } else if constexpr (std::ratio_equal_v<Period, usecs::period>) {
    os << lv.count() << "us";
  } else if constexpr (std::ratio_equal_v<Period, msecs::period>) {
    os << lv.count() << "ms";
  } else if constexpr (std::ratio_equal_v<Period, secs::period>) {
    os << lv.count() << "s";
  } else if constexpr (std::ratio_equal_v<Period, mins::period>) {
    os << lv.count() << "min";
  } else if constexpr (std::ratio_equal_v<Period, hours::period>) {
    os << lv.count() << "h";
  } else {
    os << lv.count() << "unknown";
  }
  return os;
}
#endif // INCLUDE_CHRONO_OPERATOR_HPP
