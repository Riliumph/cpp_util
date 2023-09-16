#include <iostream>
#include <pqxx/pqxx>
// original
#include "any/py_any.hpp"
namespace py {
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

/// @brief すべてのテーブルを表現できるpy::mapへ変換する関数。
/// キー名がif文に無ければすべてstd::string型として登録される。
/// そのため、以下のDB設計さえ守られていればjoinなどを行われても対応できる。
/// 【DB仕様の制約】
/// この処理はキー名と型を紐づけるため、全テーブル定義で名前と型は統一される必要がある。
/// たとえば、userテーブルのidはinteger型だが、studentテーブルのidはtext型などは許されない。
/// user::idはuser_id、student::idはstudent_idという独自のキー名である設計が必要。
/// @param result pqxxの結果
/// @return python形式でテーブルを表現したmap
py::map
Convert2Any(pqxx::result& result)
{
  py::map data;
  for (const auto& row : result) {
    py::record record;
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
} // namespace py
