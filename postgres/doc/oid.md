# [オブジェクト識別子](https://www.postgresql.jp/document/15/html/datatype-oid.html)

オブジェクト識別子とは、PostgreSQLがDB内部に格納している様々なオブジェクト(テーブルやインデックス、関数、演算子、データ型定義などなど)を一意に識別するためのIDです。

## OIDは隠し列？

```console
postgres=# \d pg_namespace
            Table "pg_catalog.pg_namespace"
  Column  |   Type    | Collation | Nullable | Default 
----------+-----------+-----------+----------+---------
 oid      | oid       |           | not null | 
 nspname  | name      |           | not null | 
 nspowner | oid       |           | not null | 
 nspacl   | aclitem[] |           |          | 
Indexes:
    "pg_namespace_oid_index" PRIMARY KEY, btree (oid)
    "pg_namespace_nspname_index" UNIQUE CONSTRAINT, btree (nspname)
postgres=# SELECT oid, * FROM pg_namespace;
  oid  |  oid  |      nspname       | nspowner |                            nspacl                             
-------+-------+--------------------+----------+---------------------------------------------------------------
    99 |    99 | pg_toast           |       10 | 
    11 |    11 | pg_catalog         |       10 | {postgres=UC/postgres,=U/postgres}
  2200 |  2200 | public             |     6171 | {pg_database_owner=UC/pg_database_owner,=U/pg_database_owner}
 13206 | 13206 | information_schema |       10 | {postgres=UC/postgres,=U/postgres}
(4 rows)
```

こういう隠し列として持っている代表的なシステムカタログは以下。

|システムカタログ|説明|
|:--:|:--|
|pg_database|DBのメタ情報|
|pg_class|table, index, viewのメタ情報|
|pg_type|データ型のメタ情報|
|pg_proc|関数のメタ情報|
|pg_operator|演算子のメタ情報|

## 独自テーブルにも張られる？

```console
postgres=# select typname, oid from pg_type order by oid desc limit 3;
    typname    |  oid  
---------------+-------
 student       | 16390
 _student      | 16389
 user_mappings | 13556
(3 rows)
```

## テーブルの型情報を取得することもできる

```sql
SELECT 
  a.attname, t.typname 
FROM 
  pg_class c, 
  pg_namespace n,
  pg_attribute a,
  pg_type t
WHERE 
  c.relnamespace = n.oid AND 
  c.oid = a.attrelid AND 
  a.atttypid = t.oid AND
  NOT a.attisdropped AND
  n.nspname = 'postgres' AND
  c.relname = 'student' AND
  a.attname = 'age';
```

## 参考

- [オブジェクト識別子を活用する](https://lets.postgresql.jp/documents/technical/object_data_type/1)
