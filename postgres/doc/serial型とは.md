# serial型

PostgreSQL 10でリリースされた機能。

## seqenceテーブル

```console
postgres=# \d
               List of relations
 Schema |      Name      |   Type   |  Owner   
--------+----------------+----------+----------
 public | student        | table    | postgres
 public | student_id_seq | sequence | postgres
(2 rows)
```

`serial`型を内包するテーブルを構築すると自動的に構築される。  
中身は以下。

```console
postgres=# select * from student_id_seq ;
 last_value | log_cnt | is_called 
------------+---------+-----------
          1 |       0 | f
(1 row)
```

### last_value

最後に生成した値を管理している。  
あくまでシーケンスが「生成した」値を管理しているに過ぎない。  
`serial`型は内部では`integer`であるため、それさえ守れば順番を無視した値を明示的に入れることはできる。

### log_cnt

新しいWALレコードを書き込む前に残っているフェッチの数を示す。

シーケンスの状態変更はトランザクションログ（WALレコード）に書き込まれること必要がある。

> 当然WALレコードを多く書き込むとパフォーマンスは落ちる

チェックポイントの後、nextvalを最初に呼び出すと、log_cntは32になる。  
次にnextvalを呼び出すたびに減少し、0に達した場合、再び32に設定され、WALレコードが書き込まれる。

### is_called

一度でもserialの自動カウント機能を使ったことがあるかどうかを判定している。  
最初は当然f(=false)。

## dropは自動的に行われる

```console
postgres=# drop table student ;
DROP TABLE
postgres=# \d
Did not find any relations.
```

生成した状態でdropをしてみたら、見事にシーケンスも消えている。

## insertにはご注意を

普通にすべてのカラムを指定して入れてみる。

```console
postgres=# insert into student values (1, 'student1', 17, current_timestamp, current_timestamp);
INSERT 0 1
postgres=# select * from student;
 id |   name   | age |          updated_at           |          created_at           
----+----------+-----+-------------------------------+-------------------------------
  1 | student1 |  17 | 2023-09-18 14:25:38.744404+00 | 2023-09-18 14:25:38.744404+00
(1 row)
```

入った。……入った？？？  
`id`カラムは`serial`型、つまりシーケンステーブルで管理されているハズである。  
シーケンステーブルはどうなっているのか？

```console
postgres=# select * from student_id_seq ;
 last_value | log_cnt | is_called 
------------+---------+-----------
          1 |       0 | f
(1 row)
```

当然、初期値から変わっていない。

では、`serial`の機能を使ってinsertしてみよう。  
データを挿入するカラムを指定してinsertしてみた。

```console
postgres=# insert into student(name,age,updated_at, created_at) values ('student?', 17, current_timestamp, current_timestamp);
ERROR:  duplicate key value violates unique constraint "student_pkey"
DETAIL:  Key (id)=(1) already exists.
```

シーケンステーブルはどうなったか？

```console
postgres=# select * from student_id_seq ;
 last_value | log_cnt | is_called 
------------+---------+-----------
          1 |      32 | t
(1 row)
```

`is_called`フラグが立ち上がって、`log_cnt`が32になった。

この状態でもう一回同じクエリを投げる。

```console
postgres=# insert into student(name,age,updated_at, created_at) values ('student?', 17, current_timestamp, current_timestamp);
INSERT 0 1
postgres=# select * from student;
 id |   name   | age |          updated_at           |          created_at           
----+----------+-----+-------------------------------+-------------------------------
  1 | student1 |  17 | 2023-09-18 14:25:38.744404+00 | 2023-09-18 14:25:38.744404+00
  2 | student? |  17 | 2023-09-18 14:29:38.400422+00 | 2023-09-18 14:29:38.400422+00
(2 rows)
```

今度はエラーを出さずにデータが挿入された。  
失敗してもシーケンステーブルは入るみたいだ。
