#ifndef INCLUDE_ENDIAN_HPP
#define INCLUDE_ENDIAN_HPP

/// @brief Machine's endian checker
/// if machine's endian is little, return true.
/// but, PDP endian is non-correspondence
/// @return true LITTLE, false maybe BIG

bool
IsLittleEndian();

#endif // INCLUDE_ENDIAN_HPP
