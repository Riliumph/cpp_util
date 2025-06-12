#ifndef INCLUDED_STD_EXT_BIT_ENDIAN_CHECK_HPP
#define INCLUDED_STD_EXT_BIT_ENDIAN_CHECK_HPP

namespace std_ext {
namespace endian {

/// @brief Machine's endian checker
/// if machine's endian is little, return true.
/// but, PDP endian is non-correspondence
/// @return true LITTLE, false maybe BIG
bool
IsLittle();
} // namespace endian
} // namespace std_ext
#endif // INCLUDED_STD_EXT_BIT_ENDIAN_CHECK_HPP
