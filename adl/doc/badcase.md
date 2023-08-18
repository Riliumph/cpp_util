# バッドケース

グッドケースではオリジナルなクラスだったが、今度は`std`名前空間の便利クラス群をエイリアスして使って見よう

## サンプル

```c++
// map.hpp
namespace adl
{
  using value_t = boost::variant<int, std::string>;
  using record = std::unordered_map<std::string, value_t>;
  using relation = std::vector<record>;
  std::ostream& operator<<(std::ostream &, const relation &);
}

// main.cpp
int
main()
{
  ...
  std::cout << v << std::endl;
  return 0;
}

```

## コンパイルエラー

これをコンパイルすると以下のように「`operator<<`を探したものの一致する定義はなかった」というエラーになる。

```bash
src/main.cpp:16:13: error: no match for ‘operator<<’ (operand types are ‘std::ostream’ {aka ‘std::basic_ostream<char>’} and ‘adl::relation’ {aka ‘std::vector<std::unordered_map<std::__cxx11::basic_string<char>, boost::variant<int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >’})
   16 |   std::cout << v << std::endl;
      |   ~~~~~~~~~ ^~ ~
      |        |       |
      |        |       adl::relation {aka std::vector<std::unordered_map<std::__cxx11::basic_string<char>, boost::variant<int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >}
      |        std::ostream {aka std::basic_ostream<char>}
```

## 解説

これは何故か？  
今回、使ったクラスはあくまでも`std`名前空間の物である。  
よって、コンパイラはADLのルールに従い、`std`名前空間を優先して探しに行ってしまう。  
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
