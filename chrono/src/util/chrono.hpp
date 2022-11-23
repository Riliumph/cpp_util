#ifndef UTIL_CHRONO_HPP
#define UTIL_CHRONO_HPP
#include <chrono>
#include <ostream>

std::ostream&
operator<<(std::ostream&, const std::chrono::system_clock::time_point&);

#endif // UTIL_CHRONO_HPP
