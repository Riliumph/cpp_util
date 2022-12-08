// STL
#include <ctime>
// original
#include "util/chrono.hpp"

std::ostream&
operator<<(std::ostream& os, const std::chrono::system_clock::time_point& tp)
{
  auto t = std::chrono::system_clock::to_time_t(tp);
#ifdef GLOBAL
  struct tm tm;
  gmtime_r(&t, &tm);
  os << tm.tm_year << "-" << tm.tm_mon << "-" << tm.tm_mday << "T" << tm.tm_hour
     << ":" << tm.tm_min << ":" << tm.tm_sec;
#elif LOCAL
  struct tm tm;
  localtime_r(&t, &tm);
  os << tm.tm_year << "-" << tm.tm_mon << "-" << tm.tm_mday << "T" << tm.tm_hour
     << ":" << tm.tm_min << ":" << tm.tm_sec;
#else
  os << ctime(&t);
#endif
  return os;
}
