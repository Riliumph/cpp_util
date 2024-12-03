#ifndef INCLUDE_POSTGRES_OPERATOR_IO_HPP
#define INCLUDE_POSTGRES_OPERATOR_IO_HPP
// STL
#include <ostream>
// original
#include "result_set.hpp"

namespace postgres {
std::ostream&
operator<<(std::ostream&, const ResultSet&);
}
#endif // INCLUDE_POSTGRES_OPERATOR_IO_HPP
