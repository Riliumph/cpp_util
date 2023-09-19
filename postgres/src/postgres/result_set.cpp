#include "postgres/result_set.hpp"
// STL
#include <iostream>

namespace postgres {
#ifndef CONSTRUCTOR
ResultSet::ResultSet() {}

/// @brief コンストラクタ
/// @param result[in] postgresの結果
ResultSet::ResultSet(pqxx::result result)
  : result_set(result)
{
  Map(Convert2Any(result_set));
}

/// @brief デストラクタ
ResultSet::~ResultSet() {}
#endif // CONSTRUCTOR

#ifndef ACCESSOR
/// @brief Query結果を二次元マップで取得する
/// @return Query結果の二次元マップ
postgres::db_table
ResultSet::Map()
{
  return this->data;
}

/// @brief Query結果を設定する
/// デバッグ用のメソッド
/// @param data Query結果
void
ResultSet::Map(postgres::db_table data)
{
  this->data = data;
}

/// @brief Query結果を生で取得する
/// より自由なことがしたい場合のIF
/// @return Query結果
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
postgres::db_table
ResultSet::Convert2Any(pqxx::result& result)
{
  postgres::db_table data;
  for (const auto& row : result) {
    postgres::db_record record;
    for (const auto& col : row) {
      auto name = std::string(col.name());
      auto type_oid = col.type();
      switch (type_oid) {
        case OID::BOOL:
          record[name] = col.as<bool>();
          break;
        case OID::INT4:
          record[name] = col.as<int32_t>();
          break;
        case OID::INT8:
          record[name] = col.as<int64_t>();
          break;
        case OID::TEXT:
          record[name] = std::string(col.c_str());
          break;
        case OID::FLOAT4:
          record[name] = col.as<float>();
          break;
        case OID::FLOAT8:
          record[name] = col.as<double>();
          break;
        case OID::TS:
          record[name] = std::string(col.c_str());
          break;
        case OID::TSTZ:
          record[name] = std::string(col.c_str());
          break;
        default:
          std::cerr << "not supported type(" << type_oid << ")" << std::endl;
      }
    }
    data.emplace_back(record);
  }
  return data;
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
