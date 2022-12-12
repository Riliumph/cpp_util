# テスト方法

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

## TEST

```bash
(*'_')<   0 $ make test
... ... ...
... ... ...
Running main() from /home/riliumph/project/googletest/googletest/src/gtest_main.cc
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from Equal
[ RUN      ] Equal.variant_operator
[       OK ] Equal.variant_operator (0 ms)
[----------] 1 test from Equal (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (0 ms total)
[  PASSED  ] 1 test.
```
