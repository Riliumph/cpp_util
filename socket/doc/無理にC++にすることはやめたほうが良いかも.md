# Linuxを無理にC++化するのをやめよう

## FDの話

たとえば、`fd`。  
気持ち的には`fd`クラスを定義したくなる。  
こうしておけば、インスタンスの寿命で`fd`を解放してくれる。  
リソースリークが起きることはない。

```c++
// 超雑サンプル
class FD
{
public:
  FD(int fd){
    fd_ = fd
  };
  ~FD(){
    SafeClose();
  }
private: // variable
  int fd_
private: // method
  void SafeClose(){
    if (0 < fd_){
      close(fd_);
    }
  }
}
```

確かにリソースリークは防げるが、Linuxの思想に合わないことが多い。  
Linuxの変数は、このクラス外で操作される事も多い。  
この`fd`だが、`select`を使うときはどうだろうか？

```C++
Server::LoopBySelect(std::function<bool(int)> fn)
{
  FD_ZERO(&fds);
  FD_SET(server_fd, &fds);
  while (1) {
    fd_set readable = fds;
    auto updated_fd_num = select(FD_SETSIZE, &readable, 0, 0, &timeout);
    if (updated_fd_num <= 0) {
      exit(1);
    }
    if (FD_ISSET(server_fd, &readable)) {
      // Accept処理
    }
    for (auto i = 0; i < FD_SETSIZE; i++) {
      if (FD_ISSET(i, &readable)) {
        auto success = fn(i); // FD値を直接制御する
        if (!success) {
          close(i);
          FD_CLR(i, &fds);
        }
      }
    }
  }
}
```

このように、`fd`を直接制御されることがある。  
基本的に勝手に`close`されたりはせず、プログラマー側に責任を持って来る。  
そのため、エラーだった場合にデストラクタを呼ぶことで`SafeClose()`を呼び出して仕組みとして`close`することもできるにはできる。

しかし、だいぶ煩雑になってしまうので、Linuxの仕組みをクラス化することは高コストだという意識を持った方が良い。
