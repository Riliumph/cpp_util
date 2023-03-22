# Tips

## Linux特有の機能はC++でラップしない方が楽

### FDの話

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

## struct addrinfoの話

他にも`getaddrinfo()`の場合もある。

```c++
struct addrinfo* inet0 = ;
auto err = getaddrinfo(NULL, service_name.data(), &hint, &inet0);
if (err != 0) {
  return -1;
}
```

`getaddrinfo()`は第三引数に`struct addrinfo**`とダブルポインタを取る。  
これをスマートポインタにしていると`std::unique_ptr<struct addrinfo>`となる。  
これは良いだろう。

でも、この`getadrinfo()`に渡す値はどうしたらいい？  
`inet.get()`はアドレスを返すだけなので型が合わない。  
`&(inet.get())`は、一見大丈夫そうに見えるが、`inet.get()`自体は管理されていない一時変数のためアドレスが分からない。  
アドレス管理用変数を定義する必要がある。  
別に`fd`の話と同じで、やってやれないことはない。全然ない。  
でも、「そこまで頑張る必要があるのか？」は考えた方がいい。  
普通に生のポインタ変数で確保して、デストラクタで`close`や`delete`を呼ぶだけでかなりコストは下がるんじゃないか？
