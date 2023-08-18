# ADLの挙動確認

ADLとはArgument Dependent Lookupと呼ばれるルールである。  
呼び出そうとしている関数に与えた実引数が名前空間に所属している場合、その名前空間内からも、呼び出そうとしている関数を探そうとするというルールである。  
このルールの挙動を見ていこう。

## ビルド方法

### good case

コンパイルが正常終了する方法

```console
$ make goodcase
```

### bad case

コンパイルが異常終了する方法

```console
$ make badcase
src/main.cpp: In function 'int main()':
src/main.cpp:17:13: error: no match for 'operator<<' (operand types are 'std::ostream' {aka 'std::basic_ostream<char>'} and 'adl::relation' {aka 'std::vector<std::unordered_map<std::__cxx11::basic_string<char>, boost::variant<int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >'})
   17 |   std::cout << v << std::endl;
      |   ~~~~~~~~~ ^~ ~
      |        |       |
      |        |       adl::relation {aka std::vector<std::unordered_map<std::__cxx11::basic_string<char>, boost::variant<int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >}
      |        std::ostream {aka std::basic_ostream<char>}
```
