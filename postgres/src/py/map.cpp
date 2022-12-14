// STL
#include <ostream>
// original
#include "py/map.hpp"

namespace py {
/// @brief すべてのテーブルを表現できるpy::mapへ変換する関数。
/// デフォルトではテーブル定義が不明なため値の型はすべてstd::string型とする。
/// @param result pqxxの結果
/// @return すべてstd::string型となったpy::map
py::map
Convert2PyMap(pqxx::result& result)
{
  py::map data;
  for (const auto& row : result) {
    py::record record;
    for (const auto& col : row) {
      auto name = std::string(col.name());
      record[name] = col.c_str();
    }
    data.emplace_back(record);
  }
  return data;
}
} // namespace py

/// @brief なんでも格納できるpy::mapをストリームに出力する
/// 1段目の出力：1行目データを精査してヘッダーとしてカラム情報を出力する。
/// 2段目以降の出力：2行目からは格納された実データを出力する。
/// データとして非推奨な状態だが、py::mapは各レコードで各々違ったカラム情報を持つことができる。
/// この場合、カラム名とデータが一致しないことになるが非推奨状態のデータなのでサポートしない。
/// Attention
/// この入力ストリーム関数はstd名前空間の引数のため、ADLの解決時にstd名前空間で検索される。
/// py名前空間に入れないこと。
/// @param os ストリーム
/// @param lv 左辺値
/// @return ストリーム
std::ostream&
operator<<(std::ostream& os, const py::map& lv)
{
  if (lv.size() <= 0) {
    return os;
  }
  // show header
  for (const auto& col : lv.at(0)) {
    os << col.first << "\t";
  }
  os << std::endl;
  // show data
  for (const auto& row : lv) {
    for (const auto& col : row) {
      os << col.second << "\t";
    }
    os << std::endl;
  }
  return os;
}
