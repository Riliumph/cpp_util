# Chronoライブラリの改造サンプル

chronoライブラリを静的ライブラリとして生成する

## ディレクトリ構成

```bash
chrono/
├ src/
| ├ chrono/     # ライブラリファイル群
| | └ etc...
| ├ chrono.hpp  # ライブラリの代表ヘッダ
| └ main.cpp    # エントリーソース
├ Makefile
|
├ obj/
├ lib/
| └ libchrono.a # 静的ライブラリ
└ bin/
  └ chrono      # 実行ファイル
```
