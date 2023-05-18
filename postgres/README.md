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
