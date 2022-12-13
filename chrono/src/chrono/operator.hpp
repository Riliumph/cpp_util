#ifndef CHRONO_OPERATOR_HPP
#define CHRONO_OPERATOR_HPP
#include <chrono>
#include <ostream>

std::ostream&
operator<<(std::ostream&, const std::chrono::system_clock::time_point&);

#endif // CHRONO_OPERATOR_HPP
