#include "util/chrono.hpp"
// STL
#include <ctime>

std::ostream&
operator<<(std::ostream& os, std::chrono::system_clock::time_point const& lv)
{
  auto t = std::chrono::system_clock::to_time_t(lv);
  auto tm = std::gmtime(&t);
  os << tm->tm_year << "-" << tm->tm_mon << "-" << tm->tm_mday << "T"
     << tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec;
  return os;
}
