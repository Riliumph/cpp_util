#ifndef INCLUDE_POSTGRES_RESULT_SET_HPP
#define INCLUDE_POSTGRES_RESULT_SET_HPP
// STL
#include <map>
#include <ostream>
#include <string>
#include <vector>
// 3rd
#include <pqxx/pqxx>
// original
#include "define.hpp"

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
  static db_table Convert2Any(pqxx::result&);
};

} // namespace postgres
#endif // INCLUDE_POSTGRES_RESULT_SET_HPP
