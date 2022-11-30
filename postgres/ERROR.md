# Error Tips

## PostgreSQLのエラー

### 共有ディレクトリの権限がホストとコンテナで異なる

docker for windowsに以下のログが出続け、生き死にを繰り返す

```bash
postgres-db-1 | fixing permissions on existing directory /var/lib/postgresql/data ... initdb: error: could not change permissions of directory "/var/lib/postgresql/data": Operation not permitted
```

どうも、windowsのファイルシステムに対して、WSL2のbashから`docker-compose up -d`を行って共有ボリュームを作ると発生する模様。  
おそらく、認識しているファイルシステムが違うことが原因。  
linuxがext4で認識してディレクトリを構築するが、当然NTFSとはパーミッション形式が違う。  
よって、パーミッションエラーが発生していると読み取れる。

WSL2でDockerを使っているならDocker for Windowsはインストール済みであるため、Powershell側から`docker-compose up -d`を行ってやれば問題なく処理される。

## ビルドエラー

### `undefined reference to`

何故かライブラリの中に関数を見つけられない。  
`pqxx`を動的リンクしているなら、`ldd`を使って動的ライブラリを探す

```bash
# DLLが参照されていないことを確認する
$ ldd ./bin/postgres | sort -f
        libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f3a56358000)
        libpqxx-7.5.so => not found
        ... ... ...
        ... ...
        ...
```

```bash
# ライブラリパスを更新する
$ sudo ldconfig
/sbin/ldconfig.real: /usr/lib/wsl/lib/libcuda.so.1 is not a symbolic link
```

```bash
# パスが通ったことを再確認する
$ ldd ./bin/postgres | sort -f
        libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f3a56358000)
        libpqxx-7.5.so => /usr/local/lib/libpqxx-7.5.so (0x00007f3a562de000)
        ... ... ...
        ... ...
        ...
```
