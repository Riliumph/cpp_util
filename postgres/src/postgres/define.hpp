#ifndef INCLUDE_POSTGRES_DEFINE_HPP
#define INCLUDE_POSTGRES_DEFINE_HPP
// STL
#include <string>
// original
#include "variant.hpp"

namespace postgres {
enum OID
{
  BOOL = 16,
  CHAR = 18,
  INT8 = 20,
  INT2 = 21,
  INT4 = 23,
  TEXT = 25,
  FLOAT4 = 700,
  FLOAT8 = 701,
  JSON = 114,
  TS = 1114,
  TSTZ = 1184
};

using db_record = record<bool, int, long, float, double, std::string>;
using db_table = table<bool, int, long, float, double, std::string>;
}
#endif // INCLUDE_POSTGRES_DEFINE_HPP
