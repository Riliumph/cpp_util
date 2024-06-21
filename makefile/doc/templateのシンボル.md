# templateはシンボル化する？

確かめる内容は２つ

1. templateはシンボル化するのか？
1. templateの特殊化はシンボル化するのか？

templateはヘッダオンリーなのでシンボル化しないと考えている。  
特殊化をして別ソースに定義を実装したらもちろんシンボル化すると考えている。

## ディレクトリ構成

```bash
/makefile
├ src/
| ├ main.cpp
| └ sub/
|   ├ calc.cpp
|   ├ calc.hpp
|   ├ storage.cpp
|   └ storage.hpp
├ obj/
| ├ main.o
| └ sub/
|   ├ calc.o
|   └ storage.o
├ lib/
| └ libmakefile.a
└ bin/
  └ makefile
```

それぞれのソースコードはそれぞれオブジェクトにコンパイルされる。  
その後、sub配下は静的ライブラリ(.a)にビルドされる。  
最後に、main.oに対してlibmakefile.aがリンクされて実行バイナリが生成される。

## templateのシンボル

calc.cppのシンボルは存在するか？

```console
$ nm -Al obj/sub/calc.o
obj/sub/calc.o:0000000000000000 T _Z3SumIiET_S0_S0_     /cpp_util/makefile/src/sub/calc.cpp:5
```

`Sum`はtemplateの特殊化（int特殊）だが、当然定義されているのでオブジェクトにシンボルが見つけられる。  
しかし、`Subtract`はヘッダオンリーなのでシンボルが存在しない。

### ヘッダオンリーのtemplateはどこで定義される？

```console
$ nm -Al obj/main.o
obj/main.o:                 U __cxa_atexit      /usr/include/c++/12/iostream:74
obj/main.o:                 U __dso_handle      /usr/include/c++/12/iostream:74
obj/main.o:0000000000000000 V DW.ref.__gxx_personality_v0
obj/main.o:                 U _GLOBAL_OFFSET_TABLE_
obj/main.o:00000000000001a7 t _GLOBAL__sub_I_main       /cpp_util/makefile/src/main.cpp:15
obj/main.o:                 U __gxx_personality_v0
obj/main.o:0000000000000000 T main      /cpp_util/makefile/src/main.cpp:7
obj/main.o:                 U strlen    /usr/include/c++/12/bits/char_traits.h:395
obj/main.o:                 U _Unwind_Resume    /cpp_util/makefile/src/main.cpp:15
obj/main.o:                 U _Z3SumIiET_S0_S0_ /cpp_util/makefile/src/main.cpp:12
obj/main.o:0000000000000155 t _Z41__static_initialization_and_destruction_0ii   /cpp_util/makefile/src/main.cpp:15
obj/main.o:0000000000000000 W _Z8SubtractIiET_S0_S0_    /cpp_util/makefile/src/sub/calc.hpp:9
（省略）
```

template関数はヘッダをincludeして使用する。  
なので、今回はmain.cppのオブジェクトで定義されることになる。

## STLってどうなってんだろう

例えば、`std::string`はstringファイルをインクルードして使用する。
じゃあ、シンボルはユーザーソースに現れるのか？

### storage.hppのstd::stringを見る

```console
$ nm -Al obj/sub/storage.o
obj/sub/storage.o:0000000000000000 T _ZN7StorageC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE       /cpp_util/makefile/src/sub/storage.cpp:4
obj/sub/storage.o:0000000000000000 T _ZN7StorageC2ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE       /cpp_util/makefile/src/sub/storage.cpp:4
obj/sub/storage.o:                 U _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1ERKS4_      /cpp_util/makefile/src/sub/storage.cpp:5
```

`std::string`は`basic_string`だが、未定義シンボルになっている。  
実行バイナリの生成時には解決していなければならない。

### どこで定義されている？

そんなことは無くて`libstdc++.so`で定義されている。

```console
$ nm -AlD /lib/x86_64-linux-gnu/libstdc++.so.6
（省略）
/lib/x86_64-linux-gnu/libstdc++.so.6:0000000000196990 W _ZNKRSt7__cxx1115basic_stringbufIcSt11char_traitsIcESaIcEE3strEv@@GLIBCXX_3.4.29
/lib/x86_64-linux-gnu/libstdc++.so.6:0000000000198050 W _ZNKRSt7__cxx1115basic_stringbufIwSt11char_traitsIwESaIwEE3strEv@@GLIBCXX_3.4.29
/lib/x86_64-linux-gnu/libstdc++.so.6:0000000000196f40 W _ZNKRSt7__cxx1118basic_stringstreamIcSt11char_traitsIcESaIcEE3strEv@@GLIBCXX_3.4.29
/lib/x86_64-linux-gnu/libstdc++.so.6:0000000000198e10 W _ZNKRSt7__cxx1118basic_stringstreamIwSt11char_traitsIwESaIwEE3strEv@@GLIBCXX_3.4.29
/lib/x86_64-linux-gnu/libstdc++.so.6:0000000000197090 W _ZNKRSt7__cxx1119basic_istringstreamIcSt11char_traitsIcESaIcEE3strEv@@GLIBCXX_3.4.29
/lib/x86_64-linux-gnu/libstdc++.so.6:0000000000198ca0 W _ZNKRSt7__cxx1119basic_istringstreamIwSt11char_traitsIwESaIwEE3strEv@@GLIBCXX_3.4.29
/lib/x86_64-linux-gnu/libstdc++.so.6:00000000001971e0 W _ZNKRSt7__cxx1119basic_ostringstreamIcSt11char_traitsIcESaIcEE3strEv@@GLIBCXX_3.4.29
/lib/x86_64-linux-gnu/libstdc++.so.6:0000000000198f80 W _ZNKRSt7__cxx1119basic_ostringstreamIwSt11char_traitsIwESaIwEE3strEv@@GLIBCXX_3.4.29
（省略）
```

実体を持つweakシンボルであることも確認できる。

### 理由

`std::string`は`std::basic_string<char>`であるから。  
templateは使用時に実体化する。  
storage.hppで`Storage`クラスのメンバ変数に`std::string`を使ったので、このタイミングで実体化したと勘違いした？  
残念ながら、`std::string`は`/usr/include/c++/12/bits/stringfwd.h`にて`typedef basic_string<char> string`と定義されている。  
つまり、標準ライブラリ内で実体化しているのである。  
だからこそ、`libstdc++.so`という共有ライブラリにシンボルが存在する。
