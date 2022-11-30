// STL
#include <string>
// system
#include <sys/time.h>
// original
#include "grade/py_grade.hpp"

namespace py {
/// @brief Gradeテーブルを表現するマップを構築する。
/// Gradeテーブル定義外のカラムはスキップされる。
/// そのため、別のテーブルとjoinするなどした場合は表現できない
/// @param result Gradeテーブルのpqxx::result
/// @return python形式でGradeテーブルを表現したmap
py::map
Convert2Grade(pqxx::result& result)
{
  py::map grades;
  for (const auto& row : result) {
    py::record record;
    for (const auto& col : row) {
      auto name = std::string(col.name());
      if (name == "id") {
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
      }
      // 対象外カラムはスキップ
      continue;
    }
    grades.emplace_back(record);
  }
  return grades;
}
} // namespace py
