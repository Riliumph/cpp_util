#include "operator_io.hpp"

namespace postgres {
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
      os << (col.is_null() ? "(NULL)" : col.c_str()) << "\t";
    }
    os << std::endl;
  }
  return os;
}
}
