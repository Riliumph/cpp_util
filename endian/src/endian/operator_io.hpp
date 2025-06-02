#ifndef INCLUDE_ENDIAN_OPERATOR_IO_HPP
#define INCLUDE_ENDIAN_OPERATOR_IO_HPP
// STL
#include <ostream>
// endian
#include "type.hpp"

std::ostream&
operator<<(std::ostream& os, endian::bytes& lv);
#endif // INCLUDE_ENDIAN_OPERATOR_IO_HPP
