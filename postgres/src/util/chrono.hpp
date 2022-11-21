#ifndef UTIL_CHRONO_HPP
#define UTIL_CHRONO_HPP
// STL
#include <chrono>
#include <ostream>

std::ostream&
operator<<(std::ostream&, std::chrono::system_clock::time_point const&);

#endif // UTIL_CHRONO_HPP
