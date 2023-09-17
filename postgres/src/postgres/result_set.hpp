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
#include "postgres/define.hpp"

namespace postgres {

/// @brief PostgreSQLの結果を管理するクラス
class ResultSet
{
public:
  ResultSet();
  ResultSet(pqxx::result);
  ~ResultSet();
  friend std::ostream& operator<<(std::ostream&, const ResultSet&);

  db_table Map();
  void Map(db_table data);
  pqxx::result Raw();
  void Raw(pqxx::result);

private:
  pqxx::result result_set;
  db_table data;
  db_table Convert2Any(pqxx::result&);
};

std::ostream&
operator<<(std::ostream&, const ResultSet&);
} // namespace postgres
#endif // RESULT_SET_HPP
