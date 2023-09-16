#ifndef PY_MAP_HPP
#define PY_MAP_HPP
// STL
#include <map>
#include <ostream>
#include <string>
#include <vector>
// boost
#if 0
#include <variant>
#else
#include <boost/variant.hpp>
#endif
// 3rd
#include <pqxx/pqxx>

/// @brief Pythonライクなint str doubleなどカラム毎に自由な型を入れれられるmap
namespace py {
#if 0
// 2022/11/1 boost::serializationはstd::variantをエンコードできない。
using value_t = std::variant<int, double, std::string>;
#else
// boost::serializationはboost::variantをエンコードできる。
using value_t = boost::variant<bool, int, long, float, double, std::string>;
#endif
/// DBのレコードを表現する型。
/// 文字列でキー情報を持ち、バリュー情報はあらゆる型で格納することができる。
using record = std::unordered_map<std::string, value_t>;
/// DBのレコードを複数格納できる型。
/// 各行でカラム名を管理している。
/// そのため非推奨ではあるが、各行で別の列名を持つレコードを登録することができる。
/// 自由なキー名をいれられることから、table joinされても対応できる。
/// このメリットは固定クラスへ変換する実装に比べて実装変更が発生しないため利便性に勝る。
/// オブジェクト指向とは仕様確定後には強烈な利点があるが、joinなど自由な処理をする場合には
/// 自由分の定義が必要になってしまう点がデメリット。
using map = std::vector<record>;
} // namespace py

std::ostream&
operator<<(std::ostream&, const py::map&);
#endif // PY_MAP_HPP
