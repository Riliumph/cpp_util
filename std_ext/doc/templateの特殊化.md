# templateの特殊化

## 基本情報

`std::enable_if<T>`は、以下のような実装になっている。  

> 本当のコードはもっと複雑だ。

```cpp
template <bool B, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
    using type = T;
};
```

`B`が`true`の時は、下のtemplateがマッチして`type`が定義される。  
`B`が`false`の時は、下のtemplateがアンマッチして別の情報を探しに行き、上のtemplateがマッチする。

つまり、`true`/`false`の2値を「型`type`が定義されている」「型`type`が定義されていない」の2値に変換しているだけである。  
型`type`が`int`だろうが`float`だろうが何だろうがどうでもよいのである。

## エラーになるコード

```cpp
#include <iostream>
template <typename T, // T型
          typename = std::enable_if_t<std::is_integral_v<T>> // T型の制約
>
void func(T t) {
    std::cout << "is_integral_v: " << t << std::endl;
}
template <typename T, // T型
          typename = std::enable_if_t<std::is_floating_point_v<T>> // T型の制約
>
void func(T t) {
    std::cout << "is_floating_point_v: " << t << std::endl;
}

int main(){
    func(1);
    func(1.0f);
}
```

このコードはエラーになる。

```console
prog.cc:11:6: error: redefinition of 'template<class T, class> void func(T)'
   11 | void func(T t) {
      |      ^~~~
prog.cc:7:6: note: 'template<class T, class> void func(T)' previously declared here
    7 | void func(T t) {
      |      ^~~~
prog.cc: In function 'int main()':
prog.cc:38:9: error: no matching function for call to 'func(float)'
   38 |     func(1.0f);
      |     ~~~~^~~~~~
prog.cc:7:6: note: candidate: 'template<class T, class> void func(T)'
    7 | void func(T t) {
      |      ^~~~
```

エラーがtemplate初心者には分かりにくいが、以下の意味だ。

>`typename T`型に`int`だの`float`だのを入れる際に、`void func(T, void)`が二つ定義があるのでどっちを使えばいいか分からない

単純なODR違反とされる。

## 解決する方法１：テンプレート実引数を利用する

```cpp
#include <iostream>
template <typename T, // T型
          std::enable_if<std::is_integral_v<T>, int>::type = 0 //T型の制約を行いながらint型の値を取る
>
void func(T t) {
    std::cout << "is_integral_v: " << t << std::endl;
}
template <typename T, std::enable_if<std::is_floating_point_v<T>, int>::type = 0>
void func(T t) {
    std::cout << "is_floating_point_v: " << t << std::endl;
}

int main(){
    func(1);
    func(1.0f);
    func<int,1000>(1);       // ただし、こういう意味不明な
    func<float, 10000>(1.0f);// コードを書けてしまう
}
```

この方法は、templateの第２引数をNon-type template parameterにすることで解決している。  
こうすることで定義は `void func<T, int>`ということになる。  

- 第1 template parameterも関数に渡す引数から型情報を取れる。  
- 第2 template parameterの`int`部分はデフォルトで`0`が入る。

以上２点より、`func(1)`のように型の情報から解放された呼び方が可能になる。

ただし、問題はある。  
明示的にtemplate parameterを渡すことができる以上、`main`関数後半のような全く意味がない上に意味不明なコードを書けてしまう。  
今回の実装の第2 template parameterは、あくまでT型に対して制約を行っているだけである。  
なので、関数内で使っていない。  
そのため、どんなint型の値を渡しても挙動は変わらないが、そのため意味が無いコードになっている。

## 解決する方法2：enabler idiom

```cpp
#include <iostream>
extern void * enabler;
template <typename T,
          typename std::enable_if<std::is_integral_v<T>>::type *& = enabler>
void func(T t) {
    std::cout << "is_integral_v: " << t << std::endl;
}
template <typename T,
          typename std::enable_if<std::is_floating_point_v<T>>::type *& = enabler>
void func(T t) {
    std::cout << "is_floating_point_v: " << t << std::endl;
}

int main(){
    func(1);
    func(1.0f);
    // こういう意味不明のコードは書けるが、これが自力で書ける人間はこんなことしない。
    func<int, static_cast<std::enable_if<true>::type*&>(enabler)>(1);
}
```

この方法は、boostで公開されたテクニックである。  
色々議論はあったようだが、このテクニック自体は広まるべきとして広められた。  

第2 template parameterで型情報を取り扱うようにしている。  
`std::enable_if`はデフォルトで`type`を`void`にするが、それを`enabler`変数の`void*`で上書きしている。  
`void*`はすべての型に変換可能なのでデフォルト値として採用できる。  
こうすると`int`の時と比べてどうなるのか？

`int`型を取っていた時に比べて意味不明なコードを書きにくくなっている。  
例にあるように書けることは書ける。

ただし、

- 第2テンプレート引数が何型になるか理解している必要がある。
  - `std::enable_if<true>::type`自体は`void`型である。
  - 第2テンプレート引数は`void*&`型である。
- 第2テンプレート引数への渡し方を理解している必要がある。
  - `void*&`型の変数を用意する必要がある。
  - temlateの引数に値を渡す場合、外部リンケージが必要であること。
    - ローカル変数ではダメ
  - `enalber`がそれに相当することが読み切れていればそれを利用できる。

```cpp
int main(){
    // 失敗例：非型引数に型は渡せない
    func<int, void*>(1);
    // 失敗例：外部リンケージの値しかダメ
    void* a;
    func<int, static_cast<void*&>(a)>(1);
    // 正解例
    func<int, static_cast<std::enable_if<true>::type*&>(enabler)>(1);
    func<int, static_cast<void*&>(enabler)>(1); // typeの型が分かる場合はこれでもOK
}
```

## 解決する方法3：戻り値に型制約を持たせる

```cpp
#include <iostream>

template <typename T>
std::enable_if<std::is_integral_v<T>, void>::type
func(T t) {
    std::cout << "is_integral_v: " << t << std::endl;
}

template <typename T>
std::enable_if<std::is_floating_point_v<T>, void>::type
func(T t) {
    std::cout << "is_floating_point_v: " << t << std::endl;
}

int main(){
    func(1);
    func(1.0f);
}
```

戻り値に型制約を持たせて解決することができる。

> 型制約なのでtemplateの中に書いてしまいたいかも？
