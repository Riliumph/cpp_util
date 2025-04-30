#ifndef INCLUDE_ENDIAN_OPERATOR_IO_HPP
#define INCLUDE_ENDIAN_OPERATOR_IO_HPP
// STL
#include <iostream>
// original
#include "type.hpp"

namespace endian {
std::ostream&
operator<<(std::ostream& os, bytes lv);
}
#endif // INCLUDE_ENDIAN_OPERATOR_IO_HPP
