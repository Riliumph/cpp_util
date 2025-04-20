# spdlogのロガーサンプル

spdlogを使ったjson構造化ログやファイルローテーションなどを実装

## ディレクトリ構造

```bash
logger/
├ src/
| ├ logger/          # loggerライブラリファイル群
| | ├ formatters/    # formatterクラス群
| | | ├ json_formatter.cpp
| | | └ json_formatter.hpp
| | ├ formatters.hpp
| | |
| | ├ sinks/         # sinkクラス群
| | | └ timestamp_rotating_file_sink.hpp
| | ├ sinks.hpp
| | |
| | ├ common.hpp     # 共通で使うファイル群
| | ├ app_logger.cpp # アプリケーションログクラス
| | └ app_logger.hpp
| |
| ├ logger.hpp  # loggerライブラリの代表ヘッダ
| └ main.cpp    # サンプルソース
|
├ Makefile
|
├ obj/
├ lib/
| └ liblogger.a # 静的ライブラリ
└ bin/
  └ logger      # 実行ファイル
```

## ビルドに必要なもの

`chrono`ユーティリティライブラリのインストールが必要。

```console
$ cd /cpp_util/chrono
$ make all
$ sudo make install
```

## ビルド方法

```console
$ make all
```

## 実行方法

```console
$ ./bin/logger
```

## 確認方法

```console
$ ls -la | grep .log
-rw-r--r--  1 vscode vscode  174 Apr 15 18:35 app.log
-rw-r--r--  1 vscode vscode  503 Apr 15 18:35 app_2025-04-15T18-35-54.log
```

ローテーションされたファイルが生まれていることが確認できる。

## 仕様

### ロガーの仕様

ロガー仕様の詳細は[こちら](./doc/logger.md)

### ログローテーションの仕様

ローテーション仕様の詳細は[こちら](./doc/log_rotation.md)
