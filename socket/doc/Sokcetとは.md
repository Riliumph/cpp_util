# Socketとは

参考：

- [ソケットの基本](http://software.aufheben.info/contents.html?contents_key=kouza_socket01)
- [調べなきゃ寝れない！と調べたら余計に寝れなくなったソケットの話](https://qiita.com/kuni-nakaji/items/d11219e4ad7c74ece748)

## そもそもソケットとは

ソケットとは日本語のコンセントのようなものである。  

## 関数のイメージの説明

あくまでイメージ。

- `socket()` コンセントを設置する関数
- `bind()` コンセントの裏を配線する
- `listen()` コンセントからタコ足を繋ぐ
- `fork()` コンセントを刺す。他の穴を塞がないようにする
- `waitpid()` コンセントを抜く。
- `unlink` コンセント裏の配線を解体する
- `close()` コンセントを取り除く

## ソケット通信の制御

### マクロ処理

|マクロ|処理|意味|
|:--:|:--|:--|
|`FD_ZERO(&fds)`|集合を空にする|初期化|
|`FD_SET(fd, &fds)`|集合内のFDビットを1にする|集合に追加する|
|`FD_CLR(fd, &fds)`|集合内のFDビットを0にする|集合から除外する|
|`FD_ISSET(fd, &fds)`|集合内のFDビットを確認する|FDが集合に含まれているかを確認する|

### FD集合

`fd_setとは以下の構造体で定義されている。  

```c++
// select.h
typedef struct {
    long int fds_bits[16];
} fd_set;
```

64bit Linuxでは`long`型は64bitであることから、64bit * 16要素 = 1024要素。  
よって、64bit Linuxでは1ポート1024個のFDを管理できる。

`FD_SET`マクロの処理だが、

```c++
// x86_64-linux-gnu/sys/select.h
#define FD_SET(fd, fdsetp) __FD_SET (fd, fdsetp)
#define FD_CLR(fd, fdsetp) __FD_CLR (fd, fdsetp)
#define FD_ISSET(fd, fdsetp) __FD_ISSET (fd, fdsetp)
#define FD_ZERO(fdsetp)  __FD_ZERO (fdsetp)
// x86_64-linux-gnu/bits/select.h
#define __FD_SET(d, set) \
  ((void) (__FDS_BITS (set)[__FD_ELT (d)] |= __FD_MASK (d)))
#define __FD_CLR(d, set) \
  ((void) (__FDS_BITS (set)[__FD_ELT (d)] &= ~__FD_MASK (d)))
#define __FD_ISSET(d, set) \
  ((__FDS_BITS (set)[__FD_ELT (d)] & __FD_MASK (d)) != 0)
// マクロ地獄なので展開すると
fdsetp->fds_bits[fd/64] |= (__fd_mask) (1UL << (fd % 64));
```

## Tips

TIME_WAIT状態は、同一ポートを別プロセスが利用するのを防ぐためにTCP規格で規定されている。  
TIME_WAIT状態のポートと同一のポートをbindしようとすると、bindは失敗してしまう。  
それをケアするのが`SO_REUSEADDR`オプションである。
