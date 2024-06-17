# 共有ライブラリのリンクエラー

```bash
$ make all
build obj src/postgres/result_set.cpp -> obj/postgres/result_set.o
g++ -MMD -Wall -Wextra -Winit-self -std=c++17 -fPIC -g -DDEBUG_ -Isrc/ -I../chrono/src/ -I../variant/src/ -o obj/postgres/result_set.o -c src/postgres/result_set.cpp

build obj src/user/user.cpp -> obj/user/user.o
g++ -MMD -Wall -Wextra -Winit-self -std=c++17 -fPIC -g -DDEBUG_ -Isrc/ -I../chrono/src/ -I../variant/src/ -o obj/user/user.o -c src/user/user.cpp

# 動的ライブラリの生成
build lib obj/postgres/result_set.o obj/user/user.o -> libpostgres.so
g++ -MMD -Wall -Wextra -Winit-self -std=c++17 -fPIC -g -DDEBUG_ -shared -o lib/libpostgres.so obj/postgres/result_set.o obj/user/user.o -L/usr/local/lib -L../chrono/lib -L./lib -lstdc++ -lchrono -lpthread -lpqxx

# 実行バイナリの生成
build obj src/main.cpp -> obj/main.o
g++ -MMD -Wall -Wextra -Winit-self -std=c++17 -fPIC -g -DDEBUG_ -Isrc/ -I../chrono/src/ -I../variant/src/ -o obj/main.o -c src/main.cpp

build bin libpostgres.so obj/main.o -> postgres
g++ -o bin/postgres obj/main.o -L/usr/local/lib -L../chrono/lib -L./lib -lpostgres
/usr/bin/ld: obj/main.o: undefined reference to symbol '_ZNK4pqxx9sql_error5queryB5cxx11Ev'
/usr/bin/ld: /lib/x86_64-linux-gnu/libpqxx-6.4.so: error adding symbols: DSO missing from command line
collect2: error: ld returned 1 exit status
make: *** [Makefile:77: postgres] Error 1
```

動的ライブラリの生成には成功している。  
しかし、実行バイナリを生成する際にシンボルが見つからないエラーが発生している。

名前マングリングされていてシンボルが読みにくいが、`pqxx::sql_error::query::cxx11`のようなものだということが分かる。  
シンボルの詳細は分からないが、C++用のPostgreSQLのクライアントライブラリの`pqxx`であることは分かる。

## どうあってほしいのか？

今回の`libpostgres.so`は自前のライブラリなので、`libpostgres.so`が何のライブラリをリンクしているかは分かる。  
しかし、オープンソースなどのライブラリになると何をリンクしてるのか分からないので、そんなものを指定しろと言われても無理である。

`libpostgres.so`を生成する際に`-lpqxx`をリンクしているので、実行バイナリでわざわざ`-lpqxx`をリンクしたくない。  

## できるか？

`pkg-config`で依存関係を解決する必要がある。
`.pc`ファイルは自作する必要がある。。。

`make install`時にコピーする……のか？

### pcファイル

```conf
prefix=/home/path-to-your-library
exec_prefix=${prefix}
includedir=${prefix}/include
libdir=${exec_prefix}/lib

Name: mylib
Description: my library
Version: 1.0.0
Cflags: -I${includedir}/mylib
Libs: -L${libdir} -lmylib
```

## コンパイル時設定で解決する

共有ライブラリとは？ってなりそうな解決方法ではあるが、実行バイナリのコンパイル時に参照するパスを書いておくという解決方法がある。

```Makefile
$(COMPILER) -o $(BIN_ROOT)/$@ $(BIN_OBJS) $(LDFLAGS) $(LIBS) -Wl,-rpath=./lib/ -l$(BIN)
```

`-Wl,-rpath=<your-lib-path>`を設定することで、そこを強制的に見に行かせることができる。  
これにより、`ld`を設定することも必要ない。

## 参考

- [【Ubuntu】ライブラリをpkg-configで管理する](https://qiita.com/tomtum/items/25549b7d7c4ad44b0fc9)
- [Guide to pkg-config](https://people.freedesktop.org/~dbn/pkg-config-guide.html)
- [PKG_CONFIG_PATH environment variable](https://askubuntu.com/questions/210210/pkg-config-path-environment-variable)
