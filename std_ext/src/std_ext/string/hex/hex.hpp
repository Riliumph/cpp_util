#ifndef INCLUDED_STD_EXT_STRING_HEX_HEX_HPP
#define INCLUDED_STD_EXT_STRING_HEX_HEX_HPP
// STL
#include <sstream>

template<typename T>
T
HexTo(const std::string& s)
{
  T value;
  std::stringstream ss;
  ss << std::hex << s;
  ss >> value;
  return value;
}

#endif // INCLUDED_STD_EXT_STRING_HEX_HEX_HPP
