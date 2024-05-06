# redis

## clientのインストール

C言語クライアントhiredisのインストール

```console
$ wget https://github.com/redis/hiredis/archive/refs/tags/v1.2.0.tar.gz -O - | tar -zxvf -
$ cd hiredis-1.2.0
$ make
$ sudo make install
```

C++用クライアント（hiredisのラッパー）をインストール

```console
$ wget https://github.com/sewenew/redis-plus-plus/archive/refs/tags/1.3.12.tar.gz -O - | tar -zxvf -
$ cd redis-plus-plus-1.3.12
$ mkdir build && cd build
$ cmake ..
$ make
$ sudo make install
$ cd ..
```

### ハッシュ型のためのjsonライブラリ

```console
$ apt install nlohmann-json3-dev
```

## Redisへの接続

```console
$  docker compose exec redis /bin/bash
```

```console
root@redis:/data# redis-cli
127.0.0.1:6379>
```
