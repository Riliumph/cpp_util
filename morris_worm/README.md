# morris worm

昔の`gets`を再現してadminユーザーを取得する方法

## ビルド方法

```console
$ make all
```

## 実行方法

```console
$ ./bin/morris_worm
Enter username:
```

### 正常ユーザー

`username`が8文字であるので、8文字以内のユーザーを登録する。

```console
$ ./bin/morris_worm
Enter username: Alice
 User
```

### 不正ユーザー（攻撃）

`username`をbuffer over runすることで強制的にAdminユーザーへ昇格する。

```console
 $ ./bin/morris_worm
Enter username: hogefugafoobar
Admin Access Granted
```
