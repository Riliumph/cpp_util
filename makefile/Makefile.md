# makefile要素解説

の前に注意すること

- インデントはタブ文字！！  
  半角スペースではダメ。なんなの？この謎仕様。  
- 「=」と「:=」は別物！！  
  **単純展開変数**か**再帰展開変数**かどうかで書き方が違う。  

それから、Kernel用のmakefileは別に書くね

---

## コンパイラオプション

### CC/CXX - コンパイラ -

コンパイラを指定する予約語変数。

- C言語コンパイラを管理する変数`CC`のデフォルト値は`cc`
- C++言語コンパイラを管理する変数`CXX`のデフォルト値は`g++`

C++なのに`CXX = clang`と設定してしまうと`iostream`系のC++クラスライブラリが見つかりませんエラーとなる。  
正しく、`clang++`と設定しよう。  
発生して原因が分かったとき、あなたはやる気をロストしていることでしょう  

### CFLAGS/CXXFLAGS - コンパイルオプション -

- C言語のフラグ変数は`CFLAGS`
- C++言語のフラグ変数は`CXXFLAGS`

#### `-g`

デバッグ情報を埋め込んでビルドしてくれる

#### `-MMD`

ソースファイルの依存関係を中間ファイルとして出力するオプション。  
この依存関係ファイルはmakefile最後の`-include`文で読み込まれるため、依存しているヘッダファイルなどが変更された場合に自動的に再コンパイルされる。  
このスクリプト考えたやつ天才か。  

#### `-MP`

`.d`ファイルにヘッダファイル用のターゲットも出力される。  
これにより、ヘッダファイルを削除したときにコンパイルエラーになることを防ぐことができる。

#### `-Wall`

警告をすべて表示する。警告は0にしような

#### `std=c++17`

C++17を解釈可能にする

---

## リンカオプション

### LDFLAG - リンクオプション -

リンカ`ld`を呼び出す際にコンパイラに設定する予約語変数。  

> `LD`とは`ld`コマンドのことでありGNUリンカを指す。  
> `ldd`や`ldconfig`などのコマンドもそれ。

**コンパイル時のライプラリ検索パス**を設定する。  

注意するべきは、コンパイル時の検索パスであり、実行時はまた別ということである。  
リンク方法によっては、コンパイルが通っても実行できないこともある。

#### 動的リンク

`-Lxxx`と`-lxxx`で設定したライブラリを動的リンクする方法。  
`-L`はコンパイル時のライブラリ検索パスだったので、実行時には別の手順が必要になる。
動的リンクは、次の順番で共有ライブラリを検索する。

1. 環境変数`LD_LIBRARY_PATH`に設定されている場所
1. `/etc/ld.so.conf`または、`/etc/ld.so.conf.d/`配下のconfファイルに書いてある場所  
   リンク時には、上記ファイルから生成された`/etc/ld.so.cache`が参照されるため、以下のコマンドで生成・更新すること。  
   `sudo ldconfig`
1. `/usr/lib`, `/lib`, `/lib64`
1. コンパイル時に`-Wl, -rpath=xxx`で指定されたパス

どれかで解決するが、基本4なのではないだろうか。  
だって、他のやり方はシステムに干渉する可能性あるし。。。

とはいえ、4は「共有ライブラリか？」と言われれば疑問は付く。

1がダメな理由は[ここ](http://xahlee.info/UnixResource_dir/_/ldpath.html)

### LDLIBS - ライブラリ -

初期値：**`-lstdc++`**

静的リンクするライブラリ指定する。  
`-pthread`や`-lm`などがよく指定される。  
`-lxxx`を指定することで、リンカがライブラリパスから以下の優先度でライブラリを検索してくれる。

1. `libxxx.so`
1. `libxxx.a`

記述されたライブラリが更新された場合、makeコマンドは再コンパイルが必要だと認識する。  
パスが通っていない場合は、「xx.so」のように書けば使用できる。  

> [pthread](https://kaworu.jpn.org/c/pthread)  
> `-pthread`と`-lpthread`の違いは特にない模様。

## 検索パス管理変数

### INCLUDE - インクルードパス -

初期値：**`-I$(SRC_ROOT)`**

ソースファイル中の`#include`の検索パスを加えるオプション。  
複数のパスを指定する場合は、`-Ixxx -Iyyy`などのように空白を空けて-Iオプションから記述する。  
`gcc -v`オプションで次のようなログが出力されるので、どこが見れていて見れていないかが判断できる。

```console
#include "..." search starts here:
#include <...> search starts here:
 src/
 /usr/include/c++/9
 /usr/include/x86_64-linux-gnu/c++/9
 /usr/include/c++/9/backward
 /usr/lib/gcc/x86_64-linux-gnu/9/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
End of search list.
```

### EXCLUDE - 検索除外リスト -

初期値：**`.git%`**

すべてのディレクトリからソースコードを探す際に、無視したいディレクトリを書いておく。  
たとえば、`.git`ディレクトリなどは大量のファイルを管理しているため`find`の対象にしたくない。  

> ソースファイル用のディレクトリがあると`.git%`は必要ない。

---

## 実行ファイル管理変数

### BIN_ROOT - 実行ファイル格納ディレクトリ -

初期値：**`bin`**

`make clean`を実装するときに、`TARGET`をルートディレクトリ名にしてしまっていると以下の問題が発生するため必ず定義すること。  

```makefile
TARGET = $(shell basename $$(pwd))

clean:
  @rm -rf $(BIN)
```

としているとき、実行ファイルと同名フォルダが存在していてたらソースコードが入っていようが何だろうが消してしまう。  
`bin`のディレクトリを挟んでおくと、さすがに`bin`下にソースコードとか置く人はいないだろうから回避し易い。  
まぁ、そんなガバ回避策よりも`.PHONY`でなんとかしろとは思う。

### BIN - 実行ファイル -

初期値：**`$(shell basename $$(pwd))`**

実行ファイル名を指定する。

- `$(shell コマンド)`  
  コマンド: `$$(pwd)`  
  さいきょーのshell関数。シェルコマンドを実行して出力を返す。  
  今回は`command`に`$$(pwd)`が割り当てられる。  
  ぱっと見で勘違いし易いが`basename`ではない。一番後ろが引数になる。
- `$(basename 文字列)`  
  対象: `pwd`の結果  
  渡されたテキストから拡張子を削除してファイル名だけを取り出す関数。  
  今回はディレクトリ名を渡しているため、結果的には親ディレクトリ名を取得する処理になっている。

<details><summary>変数の展開順序に注意</summary>

`/bin/$(shell basename $$(pwd))`の展開順序について

`$`はエスケープ文字なので、`$$`は単純な`$`という文字として解釈される。  
bashのような内側からの数学的展開であれば、以下の順番であろう。

1. テキスト：`/bin/$(shell basename $$(pwd))` -> `$(basename $$(pwd)`が実行される
1. 実行後１：`/bin/$(shell /home/username/hoge)` -> `/home/username/hoge`が実行される

しかし、これはコンピュータサイエンスであり、数学ではない。  
一番後ろのみを引数とし、その引数に頭から準に関数を実行していくのだ。

1. テキスト：`/bin/$(shell basename $$(pwd))` -> `$(shell $$(pwd))`が実行される
1. 実行後１：`/bin/$(basename /home/username/hoge)` -> `$(basename /home/username/hoge)`が実行される
1. 実行後２；`/bin/hoge`

`` `command` `` == `$(command)`  
上記の書式は全く同じ意味。実行時に使いたいのはバッククォートとと思えば分かりやすい。
とはいえ、バッククオートはGoogle Shell Style Guideにも違反するし、Markdownのコードハイライトのバッククオートとも競合するから使わない方がいいね  
</details>

---

## ソースコード管理変数

### SRC_ROOT

ソースコードを配置しているルートディレクトリ。  
`.`でもいいが、`obj`や`.git`などのソースと無関係のディレクトリも置かれるし、分けた方が便利。  
ただし、分けるとMakefileが存在する階層とズレるので処理で頑張らないといけないことも。

### BIN_SRCS - ソースコード -

コンパイル対象を指定する要素。  

よく使われる値

- 全ファイルを検索する  
  **`$(filter-out $(EXCLUDE), $(shell find $(SRC_ROOT) -name *.cpp))`**
- 一階層だけ検索する  
  **`$(filter-out $(EXCLUDE) $(TEST_EXT), $(shell find $(SRC_ROOT) -mindepth 1 -maxdepth 1 -name "*.cpp"))`**

---

## 中間ファイル系

### OBJ_ROOT

初期値：**`obj`**

中間ファイルを配置しているルートディレクトリ。  
このディレクトリの中にソースコードツリーと全く同じツリーを構築する想定。

### BIN_OBJS/LIB_OBJS - オブジェクトファイル -

初期値：**`$(addprefix $(OBJ_ROOT)/, $(patsubst $(SRC_ROOT)/%.cpp, %.o, $(BIN_SRCS)))`**

`%.cpp, %.o`により、オブジェクトファイルのルールはSRCS内の.cpp拡張子を.oにする。  
`addprefix`も`patsubst`も解説しているためスキップ。  

#### `$(patsubst 置換前, 置換後, 対象群)`

- 置換前: `$(SRC_ROOT)/%.cpp`
- 置換後: `%.o`
- 対象群: `$(BIN_SRCS)`

`$(SRC_ROOT)`が`src/`などの場合、中間ファイルツリーは`obj/src/xxx`となってしまう。  
そこで`$(SRC_ROOT)`を削除する処理を担う。  

`%`はMakefileに置けるワイルドカード。正規表現で言うところの`.*`である。  
`$(BIN_SRCS)`の先頭には`$(SRC_ROOT)/`が付与されている前提で、それらを削除している。

#### `$(addprefix 文字列, 対象群)`

- 文字列: `$(OBJ_ROOT)`
- 対象群: `patsubst`の結果

中間ファイルは中間ファイル用ディレクトリに配置したいので、`$(OBJ_ROOT)`を追加している。  
`addprefix`関数は内部でfor文を回すため、正しくすべての`BIN_SRCS`から`SRC_ROOT`を外した値に`OBJ_ROOT`を付与することができる

---

## 依存関係オプション

### OBJS - 全オブジェクトファイル -

`DEPS`で依存関係ファイルを作るために、バイナリ用テスト用問わずにオブジェクトファイルを配列化して管理している。

### DEPS - 依存関係ファイル -

**初期値：`$(OBJECTS:.o=.d)`**

最後に`-include $(DEPS)`として必ず実行されるようにすることで自動的に依存関係が解決される。  
変な式だが、`patsubst`のエイリアスである。  

- `$(patsubst pattern, replacement, %(var))`
- `%(var:pattern=replacement)`

> [6.3.1 Substitution References](https://www.gnu.org/software/make/manual/html_node/Substitution-Refs.html)  
> [8.2 Functions for String Substitution and Analysis](https://www.gnu.org/software/make/manual/html_node/Text-Functions.html)

---

## サブコマンド

makeにおけるサブコマンドの書式は以下の3要素で構成される。

```makefile
# template
target_file: dependency_file
  command to build
# 本来は$@などを使うが分かり易くするためベタ書き
app: main.o
  gcc main.o -o app
main.o: main.cpp
  gcc -c main.cpp -o main.o
```

この依存ファイル`main.o`がなければ、`main.o`をターゲットとしてコマンドが先に動く。

### .PHONY - 偽りのコマンド -

`.PHONY`はサブコマンドを実行するためだと解説されることが多い。  
この説明は厳密性に欠けていて、別に`.PHONY`が無くても**基本的には**サブコマンドは実行できる。  
`.PHONY`をコメントアウトしても`make clean`は実行できる。  
しかし、コマンドを打つパスにcleanという名前のファイルを用意すると実行できなくなる。

```bash
$ touch clean
$ make clean
make: 'clean' is up to date.
```

`make`は引数にビルド対象を取るため、`clean`をサブコマンドではなくビルド対象として認識してしまっているからである。  
この実行できないケースで`clean`をサブコマンドとして実行させるのが`.PHONY`の役割である。

```makefile
.PHONY: clean
clean:
  @rm -rf $(OBJ_ROOT) $(BIN_ROOT)
```

### allコマンド

すべてのオブジェクトをビルドするコマンド。  
決して、`all`というファイルを生成するわけではない。  
allコマンドは依存関係ファイルのビルドが自動で走ることを利用するため、依存関係の書き方は必ず`$(TARGET)`が一番最後になる。

### cleanコマンド

`make`コマンドで生成したファイルをすべて削除するコマンド。  
ソースファイルの隣にオブジェクトファイルを生成するルールだった場合、ワイルドカードや`shell`コマンドを駆使する必要がある。  
オブジェクトファイル用ディレクトリなどを用意しておくことで楽に記述出来る。  
よくプログラムでは「アルゴリズムではなくデータ構造で解決する」などと言ったりするが、今回は「アルゴリズムではなくディレクトリ構造で解決する」と言った風か。  
どちらにせよ、処理を頑張るのではなく、処理を楽にするためにデータ保存方法を工夫している。

## パターンルール

### `$(OBJ_ROOT)/%.o: $(SRC_ROOT)/%.cpp`

`$(OBJ_ROOT)/%.o`を生成するために`$(SRC_ROOT)/%.cpp`をビルドするコマンド。  
ファイルが無限に増えると一つ一つ書いてられないので、ワイルドカードを使って自動化している。

コマンドが呼ばれる流れを追うと分かり易い。  

1. `all`
1. `$(OBJ)`
   1. obj/main.oが取り出され、該当する↓のコマンドが呼ばれる
1. `$(OBJ_ROOT)/%.o: $(SRC_ROOT)/%.cpp`
   1. obj/main.o: src/main.cppに置換されてコマンドが実行される。

## makefileの組み込み機能

### 特殊な書式について - 自動変数編 -

|書式|意味|
|:-:|:---|
|`$@`|ターゲットファイル名|
|`$%`|ターゲットがアーカイブメンバだったときのターゲットメンバ名|
|`$<`|最初の依存するファイルの名前|
|`$?`|ターゲットより新しいすべての依存するファイル名|
|`$^`|すべての依存するファイルの名前|
|`$+`|Makefileと同じ順番の依存するファイルの名前|
|`$*`|サフィックスを除いたターゲットの名前|

### 特殊な書式について- 関数編 -

> [ここをみよ](https://www.ecoop.net/coop/translated/GNUMake3.77/make_8.jp.html)

テキトーなのについて解説入れてみた

| 書式 | 意味 |
|:--|---|
|`%`| ワイルドカード|
|`subst` | 置換動作|
|`patsubst` | 置換動作，ワイルドカードあり|
|`strip` | 空白文字の削除|
|`findstring` | 文字列を探す|
|`filter` | 一致する単語の削除|
|`filter-out` | 一致しない単語の削除|
|`sort` | ソートする|
|`dir` | ディレクトリ部分の抽出|
|`nodir` | ファイル部分の抽出|
|`suffix` | サフィックス（拡張子）部分|
|`basename` | サフィックス以外|
|`addsuffix` | サフィックスを加える|
|`addprefix` | プレフィックスを加える|
|`join` | 単語の連結|
|`word` | n番目の単語を返す|
|`worldlist` | 単語のリストを返す|
|`words` | 単語数を返す|
|`firstword` | 最初の名前を返す|
|`wildcard` | ワイルドカードによりファイル名リストを返す|
|`foreach` | 引数を複数回展開する|

## 参考

- [Linux共有ライブラリ「.so」の作り方](https://www.tenkaiken.com/short-articles/linux%E5%85%B1%E6%9C%89%E3%83%A9%E3%82%A4%E3%83%96%E3%83%A9%E3%83%AA-so-%E3%81%AE%E4%BD%9C%E3%82%8A%E6%96%B9/)
- [gccを用いたCの共有ライブラリの作り方](https://sleepy-yoshi.hatenablog.com/entry/20090510/p1)
- [Why LD_LIBRARY_PATH is bad](http://xahlee.info/UnixResource_dir/_/ldpath.html)
- [10.3 Variables Used by Implicit Rules](https://www.gnu.org/software/make/manual/make.html#Implicit-Variables)
