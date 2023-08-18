# ソケットの種類

参考：

- [ソケットの基本](http://software.aufheben.info/contents.html?contents_key=kouza_socket01)
- [調べなきゃ寝れない！と調べたら余計に寝れなくなったソケットの話](https://qiita.com/kuni-nakaji/items/d11219e4ad7c74ece748)

## TL,DR

|プロトコル|TCP/IP|特定方法|制約|
|:----------:|:------:|:-------:|:--|
|INET4 Domain|TCP, UDP|IPv4 + Port|ネットワーク上のホストとのIPC|
|INET6 Domain|TCP, UDP|IPv6 + Port|ネットワーク上のホストとのIPC|
|Unix  Domain|TCP, UDP|/dev/xxx|同一ホスト上のIPC|

![OSI](./img/osi-Socket.drawio.png)

## INET4 Domain

IPv4でソケット通信を行う。  
IPv4アドレスとポート番号で相手を特定する。  
IPヘッダを付与するし、TCPヘッダも付与するので通信データ多くなる。  
ともしたら輻輳制御までしてるかもしれない。  
当然、処理速度はネットワークの速度に律速する。

## INET6 Domain

INET4 Domainと基本は同じ。  
裏側ではIPv4より強化されたセキュリティ処理が動いてるのでINET4よりも重たいと思われる。

## Unix Domain

同一のUnixの中で通信されるIPC。

- pros
  - ポート番号を消費しない
  - etc...
- cons
  - 同一マシン上でのプロセス間でしか通信できない
  - スループットが高速

> [Performance Analysis of Various Mechanisms
for Inter-process Communication](http://osnet.cs.binghamton.edu/publications/TR-20070820.pdf)  
> It was hypothesized that pipes would have the highest throughput due to its limited functionality, since it is half-duplex, but this was not true. For almost all of the data sizes transferred, Unix domain sockets performed better than both TCP sockets and pipes, as can be seen in Figure 1 below.  
> On some machines, Unix domain sockets reached transfer rates as high as 1500 MB/s.

以下の３つが存在する

- ファイルシステムパス名
- 無名
- 抽象名前空間

### ファイルシステムパス名

一番一般的な手法。  
[bind(2)](http://linuxjm.osdn.jp/html/LDP_man-pages/man2/bind.2.html)を使用して、NULL終端(\0)されたファイルシステム上のパスに結びつけることができる。  
ファイルを使用しているため、パーミッションを調整する必要がある。  
サーバプロセスが終了するときには、[unlink(2)](http://linuxjm.osdn.jp/html/LDP_man-pages/man2/unlink.2.html)で名前を削除し、場合によってはそれが参照しているファイルも削除するのがお作法。

### 無名

[bind(2)](http://linuxjm.osdn.jp/html/LDP_man-pages/man2/bind.2.html)を使ってパス名に結び付けることができないストリーム型のソケットは名前を持たない。  
[socketpair(2)](http://linuxjm.osdn.jp/html/LDP_man-pages/man2/socketpair.2.html)を使うと、名前のついていない CONNECTED なソケットのペアが生成される。  
[fork(2)](https://linuxjm.osdn.jp/html/LDP_man-pages/man2/fork.2.html)してFDを繋ぎ直してあげれば、[pipe](https://linuxjm.osdn.jp/html/LDP_man-pages/man2/pipe.2.html)で実現するのと同じような双方向のプロセス間通信ができる。

### 抽象名前空間

sun_pathにファイルシステムのパスではなく、NULLバイト(\0)であることで区別される。  
NULLバイト以降に名前(文字列)を渡すが、ファイルパス名とは関係ない。  
ファイルシステムと紐付いていないので、 ファイルシステムのパーミッションなどに関係なくソケット通信できてしまう。  
Linuxでのみ利用可能。[unlink(2)](http://linuxjm.osdn.jp/html/LDP_man-pages/man2/unlink.2.html)で終了する必要がない。
