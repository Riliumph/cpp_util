// STL
#include <iostream>
// original
#include "postgres/result_set.hpp"

namespace postgres {
#ifndef CONSTRUCTOR
ResultSet::ResultSet() {}

/// @brief コンストラクタ
/// @param result[in] postgresの結果
ResultSet::ResultSet(pqxx::result result)
  : result_set(result)
{
  Map(MakeMap());
}

/// @brief デストラクタ
ResultSet::~ResultSet() {}
#endif // CONSTRUCTOR

#ifndef ACCESSOR
/// @brief Query結果を二次元マップで取得する
/// @return Query結果の二次元マップ
std::vector<ResultSet::Record>
ResultSet::Map()
{
  return this->data;
}

/// @brief Query結果を設定する
/// デバッグ用のメソッド
/// @param data Query結果
void
ResultSet::Map(std::vector<Record> data)
{
  this->data = data;
}

/// @brief Query結果を生で取得する
/// より自由なことがしたい場合のIF
/// @return Quety結果
pqxx::result
ResultSet::Raw()
{
  return this->result_set;
}

/// @brief Query結果を設定する
/// デフォルトコンストラクタを読んだときの緊急IF
/// @param result Query結果
void
ResultSet::Raw(pqxx::result result)
{
  result_set = result;
}

#endif // ACCESSOR

/// @brief
std::vector<ResultSet::Record>
ResultSet::MakeMap()
{
  std::vector<Record> result_map;
  for (const auto& row : result_set) {
    Record record;
    for (const auto& col : row) {
      // DBに名前重複は無いためチェック不要
      record[col.name()] = col.c_str();
      std::cout << col << std::endl;
    }
    result_map.emplace_back(record);
  }
  return result_map;
}

#ifndef OPERATOR_OVERLOAD
/// @brief ostream用の演算子オーバーロード
/// MakeMapされてない可能性を考慮してRawデータを用いる
/// @param os 出力ストリーム
/// @param r インスタンス
/// @return ostream
std::ostream&
operator<<(std::ostream& os, const ResultSet& r)
{
  if (r.result_set.size() <= 0) {
    // pqxx::resultはテーブル接続時にカラム名を取得するため通らない。
    return os;
  }
  // show header
  for (const auto& col : r.result_set[0]) {
    os << col.name() << "\t";
  }
  os << std::endl;
  // show data
  for (const auto& row : r.result_set) {
    for (auto col : row) {
      os << col.c_str() << "\t";
    }
    os << std::endl;
  }
  return os;
}
#endif // OPERATOR_OVERLOAD
} // namespace postgres
