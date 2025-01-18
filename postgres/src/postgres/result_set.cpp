#include "postgres/result_set.hpp"
// STL
#include <chrono>
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

/// @brief resultをPythonのmapのような形式に変換する
/// @param result 返還対象
/// @return 変換後のデータ
postgres::db_table
ResultSet::Convert2Any(pqxx::result& result)
{
  postgres::db_table data;
  try {
    for (const auto& row : result) {
      postgres::db_record record;
      for (const auto& col : row) {
        auto name = std::string(col.name());
        auto type_oid = col.type();
        if (col.is_null()) {
          record[name] = std::monostate{};
          continue;
        }
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
            record[name] = col.as<std::string>();
            break;
          case OID::FLOAT4:
            record[name] = col.as<float>();
            break;
          case OID::FLOAT8:
            record[name] = col.as<double>();
            break;
          case OID::TS:
            record[name] = col.as<std::string>();
            break;
          case OID::TSTZ:
            record[name] = col.as<std::string>();
            break;
          default:
            std::cerr << "not supported type(" << type_oid << ")" << std::endl;
        }
      }
      data.emplace_back(record);
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return data;
}
} // namespace postgres
