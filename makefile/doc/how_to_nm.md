# nmコマンドの使い方

nmコマンドはオブジェクトファイルや実行ファイルを指定することでそのオブジェクトファイルのシンボル情報を確認できる。

## 参考

- [nm - オブジェクトファイルのシンボル情報を表示する](https://linuxcommand.net/nm/#i)
- [Shohei Yoshida's Diary -  weakシンボル](https://syohex.hatenablog.com/entry/20100610/1276180481)

## シンボル表

- A/Absolute  
  絶対値であることを示す。  
  このタイプのシンボルはアドレスを持ち、静的な変数や初期化された定数に付与される。
- U/Undefined  
  未解決の外部シンボルを示す。  
  リンク時に解決される必要がある。  
  通常、外部ライブラリや他のオブジェクトで定義された関数や変数に付与される。
- T/Text  
  テキストセクションに属することを示す。  
  実行バイナリや共有ライブラリの関数のアドレスを表す。  
  実行時に直接実行されるコードのエントリポイントになる。
- B/BSS(Block Started by Symbol)  
  BSSセクションに属する未初期化データを示す。  
  通常、静的に割り当てられるメモリ領域で、プログラム開始時にゼロクリアされる。
- D/Data  
  データセクションに属することを示す。  
  静的な変数や定数のアドレスや値に付与される。
- R/Read-Only  
  `rodata`などの読み取り専用データを示す。  
  通常、定数や文字列リテラルに付与される。
- W/Weak  
  オーバーロードや名前空間を介した多重定義を示す。  
  シンボルが複数発見された場合、片方がweakシンボルである場合はもう一方（強い方）が採用される。  
  weakシンボルのみが発見された場合は、正式なシンボルとして採用される。
- V/Vector  
  ベクターテーブルエントリであることを示す。  
  仮想関数テーブル(vtable)のエントリに付与される。

## オプション

### -A / -o

出力にファイル名を付与する。

```console
$ nm -A storage.o
storage.o:0000000000000076 T _ZlsRSoRK7Storage
storage.o:0000000000000026 T _ZN7Storage4DataB5cxx11Ev
storage.o:0000000000000050 T _ZN7Storage4DataENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
storage.o:0000000000000000 T _ZN7StorageC1ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
storage.o:0000000000000000 T _ZN7StorageC2ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
storage.o:                 U _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEaSERKS4_
storage.o:                 U _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1ERKS4_
storage.o:                 U _ZStlsIcSt11char_traitsIcESaIcEERSt13basic_ostreamIT_T0_ES7_RKNSt7__cxx1112basic_stringIS4_S5_T1_EE
```

### -a

デバッグ用シンボルも表示する。

### -D

動的シンボルを表示する。

```console
$ nm -AD storage.o
nm: storage.o: no symbols
```

以下のコマンドで比較してみても面白い。  
`makefile_A.txt`では、アーカイブファイルになっている`libstorage.a`の記述がごっそり抜け落ちていることが分かる。

```console
$ nm -A  bin/makefile > makefile_A.txt
$ nm -AD bin/makefile > makefile_AD.txt
$ diff makefile_A.txt makefile_AD.txt
```

### -f

以下の中からフォーマットを指定できる。

- bsd (default)
- sysv
- posix

### -g

外部に公開されているシンボルのみ表示される。  
つまり、C/C++における`static`修飾子のようなスコープを限定するシンボルは表示されなくなる。

### --undefined-only

未定義シンボルのみ表示できる。

### --defined-only

定義済みシンボルのみ表示できる。

### -l

デバッグ情報と連携し、ファイル名:行番号のいつものフォーマットを付与できる。  
当然、undefinedなシンボルの行は表示されない。

### -n

シンボルの種類でソートを書けることができる。

### -r

`-n`の逆順

### -s

アーカイブファイルのインデックス情報が表示される。

```console
 $ nm -s ./lib/libmakefile.a 

Archive index:
_ZN7StorageC2ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in storage.o
_ZN7StorageC1ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in storage.o
_ZN7Storage4DataB5cxx11Ev in storage.o
_ZN7Storage4DataENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE in storage.o
_ZlsRSoRK7Storage in storage.o

storage.o:
（省略）
```
