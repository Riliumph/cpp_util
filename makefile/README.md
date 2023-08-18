# Makefile

`makefile`を解説するディレクトリ。  
分割コンパイルに対応したmakefileが置かれている。

googletest対応済み

## Makefileの実行手順

`make all`コマンドは`$(OBJ)`と`$(TARGET)`に依存しているため、その順番で実行される。

```bash
$ make all
[build src/main.cpp -> obj/main.o]
[build src/sub/storage.cpp -> obj/sub/storage.o]
[build obj/main.o -> makefile]
[make all]
```

## Googleテスト

### インストール手順

```bash
$ sudo apt-get install cmake
$ git clone https://github.com/google/googletest.git
$ cd googletest
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```

### 実行手順

```bash
$ make test
... ... ...
... ... ...
Running main() from /usr/src/googletest/googletest/src/gtest_main.cc
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from TEST_CASE
[ RUN      ] TEST_CASE.TEST_NAME
[       OK ] TEST_CASE.TEST_NAME (0 ms)
[----------] 1 test from Equal (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (0 ms total)
[  PASSED  ] 1 test.
```
