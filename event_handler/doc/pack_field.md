# pack_fieldとは？

packed field とは、C++の構造体やクラスで、コンパイラが通常のメモリアライメント（整列）規則を無視して、メモリを詰めて配置したフィールドのこと。  
通常、コンパイラはフィールドを高速にアクセスするために、メモリをCPUのアライメントに従って配置する（例: 4バイト境界や8バイト境界）。
しかし、#pragma pack や特定の構造体定義で「詰める」設定をすると、フィールドが連続して配置され、アライメントによる隙間（パディング）が省略されます。これが packed と呼ばれる状態である。

## pragma packで定義する

```c++
#include <iostream>

// パディングなしでフィールドを詰める設定
#pragma pack(push, 1) // アライメントを1バイト境界に設定
struct PackedStruct {
    char a;   // 1バイト
    int b;    // 通常4バイトアライメント
};
#pragma pack(pop) // 元のアライメント設定に戻す

int main() {
    std::cout << "Size of PackedStruct: " << sizeof(PackedStruct) << '\n';
    return 0;
}
// Size of PackedStruct: 5
```

通常、int型があるためアライメントは4バイトとなり、8バイトのsizeとなる。

> pragma packをコメントアウトすれば確かめられる

しかし、メモリ節約のため、4バイトアライメントを壊して1バイトアライメントにしている。

## どんな弊害が？コンパイルエラー

```c++
    for (int i = 0; i < updated_fd_num; ++i) {
      auto event = events[i];
      auto key = std::make_pair(event.data.fd, event.events);
      auto it = reaction.find(key);
    }
```

```console
src/event/epoll_handler.cpp:143:44: error: cannot bind packed field ‘event.epoll_event::data.epoll_data::fd’ to ‘int&’
  143 |       auto key = std::make_pair(event.data.fd, event.events);
```

C++標準では、参照型は「アライメントされたメモリ領域」を前提としているため、packed field を参照として扱うことができない。  

## 解決策

変数のアクセスにおいて、以下のどちらかを使用する。

- `static_cast<T>`を使ってint型扱いを行う
- `std::make_xx`などのmakeメソッドではなく、`std::pair<int, uint32_t>`などの型を明示したコンストラクタを使う
