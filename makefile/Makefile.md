# makefile要素解説

の前に注意すること  

- インデントはタブ文字！！  
  半角スペースではダメ。なんなの？この謎仕様。  
- 「=」と「:=」は別物！！  
  **単純展開変数**か**再帰展開変数**かどうかで書き方が違う。  

それから、Kernel用のmakefileは別に書くね

---

## コンパイラオプション

### COMPILER - コンパイラ -

**初期値：`g++ -v`**

C言語の時は、gcc/clangでC++の時はg++/clang++  
C++なのにclangだとiostream系のC++クラスライブラリが見つかりませんエラーとなる。  
発生して原因が分かったとき、あなたはやる気をロストしていることでしょう  

### FLAGS - コンパイルオプション -

**初期値：`-g -MMD -MP -Wall -std=c++17`**

C言語のフラグは`CFLAGS`  
C++言語のフラグは`CXXFLAGS`  

- `-g`  
  デバッグ情報を埋め込んでビルドしてくれる
- `-MMD`  
  ソースファイルの依存関係を中間ファイルとして出力するオプション  
  この依存関係ファイルはmakefile最後の`-include`文で読み込まれるため、依存しているヘッダファイルなどが変更された場合に自動的に再コンパイルされる。  
  このスクリプト考えたやつ天才か  
- `-MP`  
  `.d`ファイルにヘッダファイル用のターゲットも出力される。  
  これにより、ヘッダファイルを削除したときにコンパイルエラーになることを防ぐことができる。
- `Wall`  
  警告をすべて表示する。警告は0にしような
- `std=c++17`  
  C++17を解釈可能にする

---

## リンカオプション

### LDFLAG - リンクオプション -

**初期値：`-L/usr/local/lib`**  

リンカのオプションを指定する。  
`LIBDIRS`とかではなく`LDFLAG`なのは`Makefile`にそもそも定義されている変数名を再利用したいため。

> `LD`とは`ld`コマンドのことでありGNUリンカを指す。  
> `ldd`や`ldconfig`などのコマンドもそれ。

### LIBS - ライブラリ -

**初期値：`-lstdc++`**

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

**初期値：`-I$(SRCROOT)`**

ソースファイル中の`#include`の検索パスを加えるオプション。  
複数のパスを指定する場合は、`-Ixxx -Iyyy`などのように空白を空けて-Iオプションから記述する。  
`gcc -v`オプションで次のようなログが出力されるので、どこが見れていて見れていないかが判断できる。

```bash
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

### EXCLUDE

**初期値：**

すべてのディレクトリからソースコードを探す際に、無視したいディレクトリを書いておく。  
たとえば、`.git`ディレクトリなどは大量のファイルを管理しているため`find`の対象にしたくない。  

> ソースファイル用のディレクトリがあると`.git%`は必要ない。

---

## 実行ファイル管理変数

### BINROOT - 実行ファイル格納ディレクトリ -

**初期値：`bin`**

`make clean`を実装するときに、`TARGET`をルートディレクトリ名にしてしまっていると以下の問題が発生するため必ず定義すること。  

```makefile
TARGET = $(shell basename $$(pwd))

clean:
  @rm -rf $(BIN)
```

としているとき、実行ファイルと同名フォルダが存在していてたらソースコードが入っていようが何だろうが消してしまう。  
`bin`のディレクトリを挟んでおくと、さすがに`bin`下にソースコードとか置く人はいないだろうから回避し易い。  
まぁ、そんなガバ回避策よりも`.PHONY`でなんとかしろとは思う。

### TARGET - 実行ファイル -

**初期値：`$(shell basename $$(pwd))`**

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

> `/bin/$(shell basename $$(pwd))`の展開タイミングに注意。  
> `$`はエスケープ文字なので、`$$`は単純な`$`という文字として解釈される。  
> bashのような内側からの数学的展開であれば、以下の順番であろう。
>
> 1. テキスト：`/bin/$(shell basename $$(pwd))` -> `$(basename $$(pwd)`が実行される
> 1. 実行後１：`/bin/$(shell /home/username/hoge)` -> `/home/username/hoge`が実行される
>
> しかし、これはコンピュータサイエンスであり、数学ではない。  
> 一番後ろのみを引数とし、その引数に頭から準に関数を実行していくのだ。
>
> 1. テキスト：`/bin/$(shell basename $$(pwd))` -> `$(shell $$(pwd))`が実行される
> 1. 実行後１：`/bin/$(basename /home/username/hoge)` -> `$(basename /home/username/hoge)`が実行される
> 1. 実行後２；`/bin/hoge`
>
> `` `command` `` == `$(command)`  
> 上記の書式は全く同じ意味。実行時に使いたいのはバッククォートとと思えば分かりやすい。
> とはいえ、バッククオートはGoogle Shell Style Guideにも違反するし、Markdownのコードハイライトのバッククオートとも競合するから使わない方がいいね  

---

## ソースコード管理変数

### SRCROOT

**初期値：`src` or `.`**

ソースコードを配置しているルートディレクトリ。  
`.`でもいいが、`obj`や`.git`などのソースと無関係のディレクトリも置かれるし、分けた方が便利。  
ただし、分けるとMakefileが存在する階層とズレるので処理で頑張らないといけないことも。

### SRCS - ソースコード -

**初期値：`$(filter-out $(EXCLUDE), $(shell find $(SRCROOT) -name *.cpp))`**

コンパイル対象を指定する要素。  
`SRCROOT`以下から拡張子cppすべてをコンパイル対象とする。  

### SRCDIRS - ソースコードディレクトリ -

**初期値：`$(dir $(SRCS))`**

ソースコードが含まれているディレクトリすべてを管理する。  
区切り文字は半角スペース。

- `$(dir 文字列)`  
  文字列: `/path/xxx`  
  文字列からディレクトリを取得する（リスト対応）。  
  一番後ろの`/xxx`を削除しているだけでディレクトリの認識はしていない。  

---

## 中間ファイル系

### OBJROOT

**初期値：`obj`**

中間ファイルを配置しているルートディレクトリ。  
このディレクトリの中にソースコードツリーと全く同じツリーを構築する想定。

### OBJDIRS - 中間ファイルディレクトリ -

**初期値：`$(addprefix $(OBJROOT)/, $(patsubst $(SRCROOT)/%, %, $(SRCDIRS)))`**

中間ファイルを配置するディレクトリを格納する。  
このフォルダにオブジェクトファイルだったり中間ファイルが生成される。  
`OBJROOT`の下にソースコードのディレクトリツリーをそのまま構築する。  

`${OBJROOT}/${SRCDIRS}`と書きたいところだが、`SRCDIRS`は**スペース区切りのリスト**であるため巧く動作しない。  
`SRCDIRS`が`foo bar`だった場合、`${OBJROOT}/${SRCDIRS}`では`${OBJROOT}/foo bar`として解釈される。  
`addprefix`関数は内部でfor文を回すため、正しくすべての`SRCDIRS`に`OBJROOT`を付与することができる。

- `$(patsubst 置換前, 置換後, 対象群)`  
  置換前: `$(SRCROOT)/%`  
  置換後: `%`  
  対象群: `$(SRCDIRS)`  
  `$(SRCROOT)`が`src/`などの場合、中間ファイルツリーは`obj/src/xxx`となってしまう。  
  そこで`$(SRCROOT)`を削除する処理を担う。  
  `%`はMakefileに置けるワイルドカード。正規表現で言うところの`.*`である。  
  `$(SRCDIRS)`の先頭には`$(SRCROOT)/`が付与されている前提で、それらを削除している。
- `$(addprefix 文字列, 対象群)`  
  文字列: `$(OBJROOT)`  
  対象群: `patsubst`の結果  
  中間ファイルは中間ファイル用ディレクトリに配置するため、`$(OBJROOT)`を追加している。

### OBJS - オブジェクトファイル -

**初期値：`$(addprefix $(OBJROOT)/, $(patsubst %.cpp, %.o, $(SRCS)))`**

`%.cpp, %.o`により、オブジェクトファイルのルールはSRCS内の.cpp拡張子を.oにする。  
`addprefix`も`patsubst`も解説しているためスキップ。  

---

## 依存関係オプション

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
main.o: main.cpp main.hpp
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
  @rm -rf $(OBJROOT) $(BINROOT)
```

### $(OBJROOT)/%.o: $(SRCROOT)/%.cpp

`$(OBJROOT)/%.o`を生成するために`$(SRCROOT)/%.cpp`をビルドするコマンド。  
ファイルが無限に増えると一つ一つ書いてられないので、ワイルドカードを使って自動化している。

### allコマンド

すべてのオブジェクトをビルドするコマンド。  
決して、`all`というファイルを生成するわけではない。  
allコマンドは依存関係ファイルのビルドが自動で走ることを利用するため、依存関係の書き方は必ず`$(TARGET)`が一番最後になる。

### cleanコマンド

`make`コマンドで生成したファイルをすべて削除するコマンド。  
ソースファイルの隣にオブジェクトファイルを生成するルールだった場合、ワイルドカードや`shell`コマンドを駆使する必要がある。  
オブジェクトファイル用ディレクトリなどを用意しておくことで楽に記述出来る。

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
