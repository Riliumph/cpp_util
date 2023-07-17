# 解説

## ビルド

```console
$ docker compose build --no-cache
[+] Building 0.0s (0/1)
[+] Building 1.1s (6/6) FINISHED
[+] Building 34.5s (11/11) FINISHED
```

## 起動

```console
$ docker compose up -d
[+] Running 2/2
 ✔ Container postgres-builder-1  Started   1.3s
 ✔ Container postgres-db-1       Started   1.0s
```

## デバッグ

```console
$ docker compose exec -it builder /bin/bash
dockeruser@builder(10.10.30.2 10.10.10.3):/code make all
```

VScodeを使う場合、remote-containerプラグインによってコンテナに接続する。  
F5デバッグ、コードジャンプなども可能。

## PostgreSQLサーバーへのログイン

### コンテナに直接入る方法

```console
$ docker compose exec -it db /bin/bash
root@db_server(10.10.10.2):/ # psql -h 0.0.0.0 -p 5432 -U postgres
psql (15.3)
Type "help" for help.

postgres=#
```

### DBeaverなどでホストから入る場合

ホスト環境のポート番号5432へポートフォワードしているためホスト環境にインストールされているものと同様の方法で接続可能
