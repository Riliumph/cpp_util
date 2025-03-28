# Generator

C++20で実装される`generator`ライブラリを使わずに自作してみた。  
PythonのGeneratorのような挙動を取らせることに成功した。

`std::next`に対応するため、`Generator::Iterator`は`iterator`用の専用型（`value_type`や`iterator_category`など）を定義している。

## ビルド方法

```console
$ make all
```

## 実行方法

```console
$ ./bin/generator
=== increment sample ===
Start
1
Start -> MIDDLE
Middle
2
MIDDLE -> END
Middle
2

=== std::next() sample ===
Start
1
Start -> MIDDLE
Middle
2
MIDDLE -> END
End
3
```

後置インクリメントが正しく動作していないように思われる。  
MIDDLEからENDに遷移するときは後置インクリメントを使っているので、インスタンスは確かに前に進が、`it++`が返すインスタンスは現在の状態（MIDDLE）のままを期待してる。

うーん。
