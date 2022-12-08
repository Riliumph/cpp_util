// original
#include "any/py_any.hpp"

namespace py {
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
      if (name == "id") {
        record[name] = std::stoi(col.c_str());
      } else if (name == "name") {
        record[name] = col.c_str();
      } else if (name == "age") {
        record[name] = std::stoi(col.c_str());
      } else if (name == "student_id") {
        record[name] = col.c_str();
      } else if (name == "test_name") {
        record[name] = col.c_str();
      } else if (name == "subject") {
        record[name] = col.c_str();
      } else if (name == "score") {
        record[name] = std::stoi(col.c_str());
      } else if (name == "modify_at") {
        record[name] = col.c_str();
      } else if (name == "created_at") {
        record[name] = col.c_str();
      } else {
        // カラム名がサポートされていなければstd::stringで格納
        record[name] = col.c_str();
      }
    }
    data.emplace_back(record);
  }
  return data;
}
} // namespace py
