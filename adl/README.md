# ADLとは

ADLとはArgument Dependent Lookupと呼ばれるルールである。  
呼び出そうとしている関数に与えた実引数が名前空間に所属している場合、その名前空間内からも、呼び出そうとしている関数を探そうとするというルールである。  
このルールの挙動を見ていこう。

## 使い方

`#define ADL_ERROR`を定義しているとコンパイルエラーを作り出す。  
コメントアウトで消すことで実行できる。

## グッドケースな例

オリジナルなクラスを標準出力するために`std::ostream`の`operator<<`を定義する事が多々ある。  
しかし、`std::ostream`は標準ライブラリのクラスなため演算子オーバーロードでは対応できない。  
標準ライブラリの中に手を突っ込むことはできないからだ。  
そこで、**非メンバの演算子オーバーロード**という機能を用いてを実装して対応する。  

この際に注意するべきことがADLである。  
グッドケースはいくらでも実装されているので、今回は実装せず、サンプルを提示するだけに留める。  

```c++
// hoge.hpp
namespace adl{
  class Data{
    int i;
  };
  std::ostream& operator<<(std::ostream&, const Data&);
} // namespace adl

// main.cpp
int main(){
  adl::Data data;
  std::cout << data << std::endl;
}
```

`Data`クラスは`adl`名前空間で定義されている。  
そのため、main.cppでは`adl::Data`という形で名前空間を辿ってアクセスしている。  
では、演算子`<<`はどうだろうか？

演算子`<<`も同様に`adl`名前空間で定義されているが、実際に使っているmain.cppでは`std::cout << data`という形で特に名前空間を辿らずに実行されている。  
本来であれば`std::cout adl::operator<< data`のような形（こんなコード見たことないが）でなければ、正しい関数は呼び出せないはずである。  
これこそがADLと呼ばれるルールが強烈に働いている場面である。  
`operator<<`の引数が`adl::Data`であることから、コンパイラは`operator<<`も同様に`adl`名前空間にあるはずだとして優先的に探しに言ってくれるのである。

さて、これが便利なだけの機能なのであれば特にメモを残したりはしない。

## バッドケース

グッドケースではオリジナルなクラスだったが、今度は`std`名前空間の便利クラス群をエイリアスして使って見よう

```c++
// map.hpp
namespace adl
{
  using value_t = boost::variant<int, std::string>;
  using record = std::unordered_map<std::string, value_t>;
  using relation = std::vector<record>;
  std::ostream& operator<<(std::ostream &, const relation &);
}
```

これをコンパイルすると以下のように「`operator<<`を探したものの一致する定義はなかった」というエラーになる。

```bash
src/main.cpp:16:13: error: no match for ‘operator<<’ (operand types are ‘std::ostream’ {aka ‘std::basic_ostream<char>’} and ‘adl::relation’ {aka ‘std::vector<std::unordered_map<std::__cxx11::basic_string<char>, boost::variant<int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >’})
   16 |   std::cout << v << std::endl;
      |   ~~~~~~~~~ ^~ ~
      |        |       |
      |        |       adl::relation {aka std::vector<std::unordered_map<std::__cxx11::basic_string<char>, boost::variant<int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >}
      |        std::ostream {aka std::basic_ostream<char>}
```

これは何故か？  
今回、使ったクラスはあくまでも`std`名前空間の物である。  
よって、コンパイラはADLのルールに従い`std`名前空間を優先して探しに言ってしまう。  
しかし、今回は`adl`名前空間に関数を定義しているため`std`名前空間にはそんな関数は用意していない。  
結果、コンパイラは正しい型が採用された関数を探し当てることができず、上記のエラーを発している。

これは正しくは、`adl`名前空間から外して定義されるべきなのだ。

```c++
// map.hpp
namespace adl
{
  using value_t = boost::variant<int, std::string>;
  using record = std::unordered_map<std::string, value_t>;
  using relation = std::vector<record>;
}
std::ostream& operator<<(std::ostream &, const relation &);
```
