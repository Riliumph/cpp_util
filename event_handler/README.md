# event handler

`epoll`を使ったイベントハンドラのサンプル

## ビルド方法

```console
$ make all
```

## 実行方法

```console
$ ./bin/event_handler
Loop Event
(入力待機中)
hello, world // 入力
fd: 0
hello, world
(ctrl+D)
EOF received. Exiting...
```

```console
$ telnet localhost 50000
```
