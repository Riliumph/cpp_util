# Socketのサンプルプログラム

## 使い方

terminalで以下のコマンドを投入する。

```terminal
$ make all
$ ./bin/socket
wait select ...
（受信待機中）
```

別のターミナルで以下のコマンドを投入する。

```terminal
$ telnet 127.0.0.1 50000
Trying 127.0.0.1...
Connected to 127.0.0.1.
Escape character is '^]'.
（入力待機中）
```

## TCPDUMP

```console
$ sudo tcpdump -i any port 50000 -X
tcpdump: data link type LINUX_SLL2
tcpdump: verbose output suppressed, use -v[v]... for full protocol decode
listening on any, link-type LINUX_SLL2 (Linux cooked v2), snapshot length 262144 bytes
```
