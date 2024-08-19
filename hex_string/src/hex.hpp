#ifndef INCLUDE_HEX_HPP
#define INCLUDE_HEX_HPP

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

#endif // INCLUDE_HEX_HPP
