#ifndef RESULT_SET_HPP
#define RESULT_SET_HPP
// STL
#include <map>
#include <ostream>
#include <string>
#include <vector>
// 3rd
#include <pqxx/pqxx>
// original
#include "py/map.hpp"

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

/// @brief PostgreSQLの結果を管理するクラス
class ResultSet
{
public:
  ResultSet();
  ResultSet(pqxx::result);
  ~ResultSet();
  friend std::ostream& operator<<(std::ostream&, const ResultSet&);

  py::map Map();
  void Map(py::map data);
  pqxx::result Raw();
  void Raw(pqxx::result);

private:
  pqxx::result result_set;
  py::map data;
  py::map Convert2Any(pqxx::result&);
};

std::ostream&
operator<<(std::ostream&, const ResultSet&);
} // namespace postgres
#endif // RESULT_SET_HPP
