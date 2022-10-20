# makefile要素解説

の前に注意すること  

- インデントはタブ文字！！  
  半角スペースではダメ。なんなの？この謎仕様。  
- 「=」と「:=」は別物！！  
  単純展開変数か再帰展開変数かどうかで書き方が違う。  

それから、Kernel用のmakefileは別に書くね  

---

## COMPILER - コンパイラ -

例えば、C言語の時は、gcc/clangでC++の時はg++/clang++  
C++なのにclangだとiostream系のC++クラスライブラリが見つかりませんエラーとなる。  
発生して原因が分かったとき、あなたはやる気をロストしていることでしょう  

## FLAGS - コンパイルオプション -

C言語のフラグは`CFLAGS`  
C++言語のフラグは`CXXFLAGS`  

- `-g`  
  デバッグ情報を埋め込んでビルドしてくれる
- `-MMD -MP`  
  ソースファイルの依存関係を中間ファイルとして出力するオプション  
  この依存関係ファイルはmakefile最後の`-include`文で読み込まれるため、依存しているヘッダファイルなどが変更された場合に自動的に再コンパイルされる。  
  このスクリプト考えたやつ天才か  

---

## LDFLAG - リンクオプション -

初期値：`（空）`  
動的ライブラリをリンクする「-l」オプションを用いる場合はここに記述することで使用できる。  
パスが通っていない場合は、「xx.so」のように書けば使用できる。  

`getconf`コマンドで32/64bitを切り替えている。  
LONG_BITとは、`long int`型のbit数を管理している全システム構成変数である。  

> 参考：[getconfコマンド](https://www.ibm.com/support/knowledgecenter/ja/ssw_aix_72/g_commands/getconf.html)

## LIBS - ライブラリ -

初期値：`（空）`  
静的リンクするライブラリ指定する。  
静的ライブラリを用いる場合、空白区切りでファイル名を記述する。  
記述されたライブラリが更新された場合、makeコマンドは再コンパイルが必要だと認識する。

---

## INCLUDE - インクルードパス -

初期値：`-I./include -I/usr/local/include`  
ソースファイル中の`#include`の検索パスを加えるオプション。  
`-I`が必ず必要なので注意。  
複数のパスを指定する場合は、`-Ixxx -Iyyy`などのように空白を空けて-Iオプションから記述する。  

`gcc -v`オプションで次のようなログが出力されるので、どこが見れていて見れていないかが判断できる。

```bash
#include "..." search starts here:
#include <...> search starts here:
 ./
 /usr/include/c++/9
 /usr/include/x86_64-linux-gnu/c++/9
 /usr/include/c++/9/backward
 /usr/lib/gcc/x86_64-linux-gnu/9/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
End of search list.
```

---

## 実行ファイル系

### BINDIR - 実行ファイル格納ディレクトリ -

初期値：`bin`  

`make clean`を実装するときに、`TARGET`をルートディレクトリ名にしてしまっていると以下の問題が発生するため必ず定義すること。  

```bash
TARGET = $(shell basename $$(readlink -f .))

clean:
  @rm -rf $(BIN)
```

としているとき、同名のフォルダが存在していてたらソースコードが入っていようが何だろうが消してしまう。  
`bin`のディレクトリを挟んでおくと、さすがに`bin`下にソースコードとか置く人はいないだろうから回避し易い。

### TARGET - 実行ファイル -

実行ファイル名を指定する。  
初期値：`$(shell basename $$(readlink -f .))`  

> `/bin/$(shell basename $$(readlink -f .))`の展開タイミングに注意。  
> `$`はエスケープ文字なので、`$$`は単純な`$`という文字として解釈される。
>
> - make実行時：`/bin/$(shell basename $$(readlink -f .))` -> `/bin/$(basename $(readlink -f .))`  
>   `$`一つが展開され、make関数である`$(shell basename $$(readlink -f .))`がbashの`$(basename $(readlink -f .))`として展開される。
> - コマンド実行時： `/bin/$(basename $(readlink -f .))` ->  `/bin/hoge`  
>   内側からbashコマンドが実行される。
>  
> ``readlink -f .`` == `$(readlink -f .)`  
> 上記の書式は全く同じ意味。実行時に使いたいのはバッククォートとと思えば分かりやすい。
> とはいえ、バッククオートはGoogle Shell Style Guideにも違反するし、Markdownのコードハイライトのバッククオートとも競合するから使わない方がいいね  

解説  

- `./bin`
- `shell`  
  さいきょーのshellコマンドを実行する。
- `basename`  
  親ディレクトリ名を取得コマンド。
- `readlink -f .`  
  カレントパスを絶対パスにして取得するコマンド。  
  `readlink -f`ってMacにはなかった気がする……あっ  

---

## ソースコード系

### SRCDIR - ソースコードディレクトリ -

### SOURCES - ソースコード -

初期値：`$(wildcard $(SRCDIR)/*.cpp)`  
コンパイル対象を指定する要素。  
SRCDIR内に存在する拡張子cppすべてをコンパイル対象とする。  
え？なに？C言語をコンパイルしたい？正気かよ……  
makefile内のcppをすべてcに変えるのと、コンパイラをgcc/clangに変えてください。  

---

## 中間ファイル系

### OBJDIR - 中間ファイルディレクトリ -

初期値：`../obj`
中間ファイルを配置するディレクトリ  
このフォルダにオブジェクトファイルだったり中間ファイルが生成される。  
この要素が空の場合、makefileと同じ階層に配置される。

### OBJECTS - オブジェクトファイル -

初期値：`$(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))`  
オブジェクトファイルの格納先は、OBJDIR  
オブジェクトファイルのルールはSOURCES内の.cpp拡張子を.oにする。  

---

## DEPENDS - 依存関係ファイル -

初期値：`$(OBJECTS:.o=.d)`  
オブジェクトファイルの拡張子を.dにしましょうねーっていう意味

---

## makefileの組み込み機能

### 特殊な書式について - 自動変数編 -

| 書式 | 意味 |
|:-:|---|
|$@ | ターゲットファイル名|
|$% | ターゲットがアーカイブメンバだったときのターゲットメンバ名|
|$< | 最初の依存するファイルの名前|
|$? | ターゲットより新しいすべての依存するファイル名|
|$^ | すべての依存するファイルの名前|
|$+ | Makefileと同じ順番の依存するファイルの名前|
|$* | サフィックスを除いたターゲットの名前|

### 特殊な書式について- 関数編 -

まぁ、[ここをみよ](https://www.ecoop.net/coop/translated/GNUMake3.77/make_8.jp.html)  
テキトーなのについて解説入れてみた  

| 書式 | 意味 |
|:-:|---|
|% | ワイルドカード|
|subst | 置換動作|
|patsubst | 置換動作，ワイルドカードあり|
|strip | 空白文字の削除|
|findstring | 文字列を探す|
|filter | 一致する単語の削除|
|filter-out | 一致しない単語の削除|
|sort | ソートする|
|dir | ディレクトリ部分の抽出|
|nodir | ファイル部分の抽出|
|suffix | サフィックス（拡張子）部分|
|basename | サフィックス以外|
|addsuffix | サフィックスを加える|
|addprefix | プレフィックスを加える|
|join | 単語の連結|
|word | n番目の単語を返す|
|worldlist | 単語のリストを返す|
|words | 単語数を返す|
|firstword | 最初の名前を返す|
|wildcard | ワイルドカードによりファイル名リストを返す|
|foreach | 引数を複数回展開する|
