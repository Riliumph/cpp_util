# makeがmakeを呼ぶサンプル

## ビルド方法

```console
$ make all
```

## 実行方法

`wl, rpath`に指定した相対パスが解決できるパスで実行する方法。

```console
 $ ./bin/greet 
```

実行パスから見て動的ライブラリが見つかるようにパスを設定して実行する方法。

```console
$  LD_LIBRARY_PATH=./human/lib:$LD_LIBRARY_PATH ./greet/bin/greet
```

## 実験内容について

[ビルド結果](./ビルド結果.md)
[実行結果](./実行結果.md)
