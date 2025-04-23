# Socketのサンプルプログラム

## ビルド方法

前提として以下のライブラリをインストールする必要がある。

```console
$ cd /cpp/util/event_handler
$ make all
$ sudo make install
```

```console
$ cd /cpp_util/endian
$ make all
$ sudo make install
```

### サーバー

```console
$ make -f server.Makefile all
```

### クライアント

```console
$ make -f client.Makefile all
```

## 使い方

### TCPサーバー

terminalで以下のコマンドを投入する。

```terminal
$ ./bin/server tcp 127.0.0.1 50000
```

別のターミナルで以下のコマンドを投入する。

```terminal
$ telnet 127.0.0.1 50000
Trying 127.0.0.1...
Connected to 127.0.0.1.
Escape character is '^]'.
（入力待機中）
```

### UDPサーバー

terminalで以下のコマンドを投入する。

```terminal
$ ./bin/server udp 127.0.0.1 50000
```

別ターミナルで以下のコマンドを投入する

```terminal
$ echo "HELLO, WORLD" | nc -u 127.0.0.1 50000
```

## TCPDUMP

```console
$ sudo tcpdump -i any port 50000 -X
tcpdump: data link type LINUX_SLL2
tcpdump: verbose output suppressed, use -v[v]... for full protocol decode
listening on any, link-type LINUX_SLL2 (Linux cooked v2), snapshot length 262144 bytes
```
