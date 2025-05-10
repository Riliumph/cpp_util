# Fork実験

fork+execveによる子プロセスを生成し、それをgdbでデバッグする実験

## ビルド方法

```console
$ make all
```

## 実行方法

```console
$ ./bin/fork ./bin/fork_sub hoge 1234
```

## 確認方法

起動直後の`ps`コマンド結果。  
`./bin/fork`がforkシステムコールにより自身を複製したことが分かる。

```console
$ ps aux | grep fork
vscode    6504  0.0  0.0   2464   932 pts/2    S+   17:11   0:00 ./bin/fork ./bin/fork_sub hoge 1234
vscode    6505  0.0  0.0   2464   100 pts/2    S+   17:11   0:00 ./bin/fork ./bin/fork_sub hoge 1234
```

数秒後にもう一度`ps`コマンドを実行する。  
PID6505のプロセスが`./bin/fork_sub`に切り替わったことが分かる。

```console
$ ps aux | grep fork
vscode    6504  0.0  0.0   2464   932 pts/2    S+   17:11   0:00 ./bin/fork ./bin/fork_sub hoge 1234
vscode    6505  0.0  0.0   2464   896 pts/2    S+   17:11   0:00 ./bin/fork_sub hoge 1234
```
