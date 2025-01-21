# キーが二つのmap

## mapをネストする方法

```c++
#include <iostream>
#include <map>
#include <string>

using Map2D = std::map<int, std::map<int, std::string>>;

std::ostream&
operator<<(std::ostream& os, const Map2D& m)
{
  for (const auto& outer_entry : m) {
    os << "(" << outer_entry.first << ", ";
    if (outer_entry.second.empty()) {
      os << "-)" << " -> " << "(EMPTY) but exist" << std::endl;
    } else {
      for (const auto& inner_entry : outer_entry.second) {
        os << inner_entry.first << ") -> " << inner_entry.second << std::endl;
      }
    }
  }
  return os;
}

int
main()
{
  Map2D data;

  data[1][1] = "1";
  data[1][2] = "1-2";
  data[2][1] = "2-1";

  std::cout << "Before update:" << std::endl;
  std::cout << data << std::endl;

  // データの更新
  data[1][2] = "1-2: updated";

  // 更新後の表示
  std::cout << "After update:" << std::endl;
  std::cout << data << std::endl;

  // データの削除
  data[1].clear();

  // 削除後の表示
  std::cout << "After erase:" << std::endl;
  std::cout << data << std::endl;

  // 完全削除
  data[1].clear();
  if (data[1].empty()) {
    data.erase(1);
  }
  // 削除後の表示
  std::cout << "After correct erase:" << std::endl;
  std::cout << data << std::endl;
  return 0;
}
```

一番簡単に思いつく例で、`std::map<T,U>`をネストする方法。  
この方法は、`data[1][2]`と配列のようにアクセスが可能である。  
他方、`std::map`を2回検索することになるので`O(log n)`の計算が2回発生する。

また、削除する方法が特殊となる。

```c++
  data[1].clear();
  if (data[1].empty()) {
    data.erase(1);
  }
```

`std::map`は2層目のデータが空になったからと言って、その要素を消してくれる訳ではない。  
さらに、カスタムデリータを入れるような構造にもなっていないので、自動化もできない。

可読性は高いが、削除などに落とし穴が残る。

特に、キーが3つになると面倒すぎる気がする。

## std::pair<T,U>を使う方法

```c++
#include <iostream>
#include <map>
#include <string>
#include <utility>  // std::pair

template<typename T>
using Map2D = std::map<std::pair<int, int>, T>;

template<typename T>
std::ostream& operator<<(std::ostream& os, const Map2D<T>& m) {
    for (const auto& entry : m) {
        os << "(" << entry.first.first << ", " << entry.first.second << ") -> " << entry.second << std::endl;
    }
    return os;
}

int main() {
    Map2D<std::string> data;

    data[{1, 2}] = "first";
    data[{3, 4}] = "second";

    std::cout << "Before update:" << std::endl;
    std::cout << data << std::endl;

    // データの更新
    data[{1, 2}] = "updated_first";

    // 更新後の表示
    std::cout << "After update:" << std::endl;
    std::cout << data << std::endl;

    // データの削除
    data.erase({1,2});  // (5, 6) のエントリを削除

    // 削除後の表示
    std::cout << "After erase:" << std::endl;
    std::cout << data;

    return 0;
}
```

`std::pair`はキーが3つになると使えない問題はある。  
しかし、ネストした場合と違って1層目が空だが存在するようなことは発生しない。

ただし、階層を作っていないので第一要素1をすべて削除するといったことはできない。
