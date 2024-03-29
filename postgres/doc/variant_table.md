# variant table型とは

Pythonのmapのように、DBからのデータをなんでも格納できる型がほしい！！として作ったデータ型。

## 静的型付けがいいか？動的型付けがいいか？

一番初めにRDBMSのデータをC++で扱おうとした場合、テーブル単位でクラスを作ろうとしていないだろうか？  
最近だと、json（NoSQL）のデータをそのままマップに落とし込むGo言語が出てきたのでそういう意見も少ないだろうか。

さておき、RDBMSのテーブルをクラスに興す際のは非常に大変である。  
RDBMSは1:N関係などを表現するときにテーブルを分けたり中間テーブルを挟んだする。  
そして、お目当てのデータを一気に使う場合は、それらのテーブルをjoinすることで、その時その場で一番適切なテーブルを作成する方法を取る。

この性質が静的型付け言語と相性が合わないと感じている。

実行されるまでテーブル定義が分からないのだ。  
コンパイルタイミングで型を確定させるようなクラスが書けるハズがない。

そこでGo言語の思想である、なんでも入るMapを作ればいいとなった。

DBのデータ型とは、一見するよりも非常に複雑なデータであることを意識せよ。  

### メリット

なんでも格納できる共用体を使うため、その場しのぎにjoinされたテーブルでもデータ定義を変更する必要はない。  
つまり、テーブルのjoinパターンが無限にあれば無限に型定義を行わなければならないが、Mapを使うことで１つの定義で済むという低コストが
メリットである。

### デメリット

クラス定義をする場合に比べて、メンバ変数でのアクセスができない。  
アクセスの都度、キー検索をしてバリューを得るという演算が必要になる。

パフォーマンスは優れていない。

また、テーブル構造を作ろうとすると、マップを格納する配列を用いることになる。  
１行１行がMap構造を取るので、前後でマップ構造（キー）が異なっても格納できてしまう。  
この自由度がメリットではあるが、やろうと思えばすべての行で異なるデータを格納できてしまうのが扱う上での注意事項となる。
