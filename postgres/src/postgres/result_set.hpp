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
// #include "postgres/record.hpp"

namespace postgres {
/// @brief PostgreSQLの結果を管理するクラス
class ResultSet
{
public:
  /// @brief 一レコードを表現するクラス
  using Record = std::map<std::string, std::string>;

  ResultSet();
  ResultSet(pqxx::result);
  ~ResultSet();
  friend std::ostream& operator<<(std::ostream&, const ResultSet&);

  std::vector<Record> Map();
  void Map(std::vector<Record> data);
  pqxx::result Raw();
  void Raw(pqxx::result);

private:
  pqxx::result result_set;
  std::vector<Record> data;

  std::vector<Record> MakeMap();
};

std::ostream&
operator<<(std::ostream&, const ResultSet&);
} // namespace postgres
#endif // RESULT_SET_HPP
