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
