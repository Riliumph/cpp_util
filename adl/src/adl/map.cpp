// original
#include "adl/map.hpp"

namespace adl
{
#ifdef ADL_ERROR
  std::ostream &
  operator<<(std::ostream &os, const relation &lv)
  {
    if (lv.size() <= 0)
    {
      return os;
    }
    // show header
    for (const auto &col : lv.at(0))
    {
      os << col.first << "\t";
    }
    os << std::endl;
    // show data
    for (const auto &row : lv)
    {
      for (const auto &col : row)
      {
        os << col.second << "\t";
      }
      os << std::endl;
    }
    return os;
  }
#endif
} // namespace adl

#ifndef ADL_ERROR
std::ostream &
operator<<(std::ostream &os, const adl::relation &lv)
{
  if (lv.size() <= 0)
  {
    return os;
  }
  // show header
  for (const auto &col : lv.at(0))
  {
    os << col.first << "\t";
  }
  os << std::endl;
  // show data
  for (const auto &row : lv)
  {
    for (const auto &col : row)
    {
      os << col.second << "\t";
    }
    os << std::endl;
  }
  return os;
}
#endif
