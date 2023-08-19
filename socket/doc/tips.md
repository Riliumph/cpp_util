# Tips

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
