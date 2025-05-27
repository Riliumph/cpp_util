# event handler

`epoll`を使ったイベントハンドラのサンプル

## ビルド方法

```console
$ make all
```

## 実行方法

```console
$ ./bin/event_handler epoll
```

### 標準入力

```console
$ ./bin/event_handler epoll
waiting event by epoll ...
（省略：入力待機中）
hello, world!
event(fd(0), event(1)) fired!!
===== RECEIVED EVENT(0) =====
hello, world!
waiting event by epoll ...
```

### ソケット通信

別ターミナルでイカを実行する

```console
$ telnet localhost 50000
```

### シグナル

プロセスが動いているターミナルで`ctrl+c`を実行する。  

```console
waiting event by epoll ...
（省略：入力待機中）
^Cevent(fd(4), event(1)) fired!!
Received SIGINT (Ctrl+C)
waiting event by epoll ...
```

終了処理を実装していないため、終了しない。  
または

```console
$ ps aux | grep event_handler
vscode    5204  0.0  0.0   5740  1876 pts/1    S+   17:46   0:00 ./bin/event_handler epoll
$ kill 5204
```

```console
waiting event by epoll ...
event(fd(4), event(1)) fired!!
Received SIGTERM
```

## 終了方法

```console
$ kill -9 <PID>
```

```console
waiting event by epoll ...
Killed
```
