# インストールについて

ビルドしたあと、成果物をインストールすることもあるだろう。  
例えば、ライブラリ（.so）とそのヘッダー（.h）などである。

そのルールを見ていこう

## 方法１：コマンドでやっちゃう

まず、初めに思いつくのはコマンドで一気にコピーする方法だ。

```makefile
install:
    cp --parent $(LIBS) $(LIBS_INS_PATH)
    cp --parent $(HEADS) $(HEADS_INS_PATH)
```

これでも問題はない。

## 方法２：パターンルールで解決

`cp`コマンドにディレクトリツリーを維持したままコピーするオプションがあるので方法１でも問題ない。  
しかし、これをパターンで何とかする方法もある。

```makefile
install_heads: $(INS_LIB_HEADS)
   @echo "install headers"

# header copy rule
$(HEAD_INS_PATH)/%: $(SRC_ROOT)/%
   @[ -d $@ ] || mkdir -p $(dir $@)
   @cp -v $< $@

install_libs: $(LIBS)
 @echo "install libs"

# lib copy rule
$(LIBS_INS_PATH)/%: $(LIB_ROOT)/%
   @[ -d $@ ] || mkdir -p $(dir $@)
   @cp -v $< $@
```

- `INS_LIB_HEADS`は `xxx/hoge/fuga.hpp xxx/hoge/foo.hpp`などのファイルリスト
- `HEADS_INS_PATH`は `xxx/hoge xxx/huga`などのディレクトリリスト

`HEADS_INS_PATH/%`は、`INS_LIB_HEADS`のファイル部分を除いた文字列に一致するためパターンとして動くことが出来る。  

内容は、

1. そのファイルリストの要素が存在していなければディレクトリを作成する。
2. ファイルを該当パスにコピーする
