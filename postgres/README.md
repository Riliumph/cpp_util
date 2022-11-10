# 解説

## インストール

PostgreSQLの接続ライブラリをインストール

```bash
$ sudo apt install libpqxx-dev
$ sudo apt install postgresql-client
$ ls /usr/include | grep pqxx
drwxr-xr-x   3 root root 4.0K yy-mm-dd hh:mm:ss pqxx/
```

## PostgreSQLの実行・接続

```bash
$ docker-compose up -d
[+] Running 2/2
 ⠿ Network postgres_default  Created  0.7s
 ⠿ Container postgres-db-1   Started  1.3s
$ psql -h 0.0.0.0 -p 5432 -U postgres
psql (12.12 (Ubuntu 12.12-0ubuntu0.20.04.1), server 14.5 (Debian 14.5-1.pgdg110+1))
WARNING: psql major version 12, server major version 14.
         Some psql features might not work.
Type "help" for help.

postgres=# 
```

## PostgreSQLコンテナサーバーへの接続

```bash
$ docker ps
CONTAINER ID   IMAGE         COMMAND                  CREATED         STATUS         PORTS                    NAMES
6ec621688b6c   postgres-db   "docker-entrypoint.s…"   5 minutes ago   Up 5 minutes   0.0.0.0:5432->5432/tcp   postgres-db-1
$ docker exec -it postgres-db-1 bash
```

## PostgreSQLのエラー

### 共有ディレクトリの権限がホストとコンテナで異なる

docker for windowsに以下のログが出続け、生き死にを繰り返す

```bash
postgres-db-1 | fixing permissions on existing directory /var/lib/postgresql/data ... initdb: error: could not change permissions of directory "/var/lib/postgresql/data": Operation not permitted
```

どうも、windowsのファイルシステムに対して、WSL2のbashから`docker-compose up -d`を行って共有ボリュームを作ると発生する模様。  
おそらく、認識しているファイルシステムが違うことが原因。  
linuxがext4で認識してディレクトリを構築するが、当然NTFSとはパーミッション形式が違う。  
よって、パーミッションエラーが発生していると読み取れる。

WSL2でDockerを使っているならDocker for Windowsはインストール済みであるため、Powershell側から`docker-compose up -d`を行ってやれば問題なく処理される。

## docker-compose

<https://zenn.dev/sarisia/articles/0c1db052d09921>

## ビルドエラー

### `undefined reference to`

何故かライブラリの中に関数を見つけられない。  
`pqxx`を動的リンクしているなら、`ldd`を使って動的ライブラリを探す

```bash
# DLLが参照されていないことを確認する
$ ldd ./bin/postgres | sort -f
        libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f3a56358000)
        libpqxx-7.5.so => not found
        ... ... ...
        ... ...
        ...
```

```bash
# ライブラリパスを更新する
$ sudo ldconfig
/sbin/ldconfig.real: /usr/lib/wsl/lib/libcuda.so.1 is not a symbolic link
```

```bash
# パスが通ったことを再確認する
$ ldd ./bin/postgres | sort -f
        libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f3a56358000)
        libpqxx-7.5.so => /usr/local/lib/libpqxx-7.5.so (0x00007f3a562de000)
        ... ... ...
        ... ...
        ...
```
