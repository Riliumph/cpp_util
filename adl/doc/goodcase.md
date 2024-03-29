
# グッドケースな例

オリジナルなクラスを標準出力するために`std::ostream`の`operator<<`を定義する事が多々ある。  
しかし、`std::ostream`は標準ライブラリのクラスなため演算子オーバーロードでは対応できない。  
標準ライブラリの中に手を突っ込むことはできないからだ。  
そこで、**非メンバの演算子オーバーロード**という機能を用いてを実装して対応する。  

この際に注意するべきことがADLである。  

## サンプル実装

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

## 解説

`Data`クラスは`adl`名前空間で定義されている。  
そのため、main.cppでは`adl::Data`という形で名前空間を辿ってアクセスしている。  
では、演算子`<<`はどうだろうか？

演算子`<<`も同様に`adl`名前空間で定義されているが、実際に使っているmain.cppでは`std::cout << data`という形で特に名前空間を辿らずに実行されている。  
本来であれば`std::cout adl::operator<< data`のような形（こんなコード見たことないが）でなければ、正しい関数は呼び出せないはずである。  
これこそがADLと呼ばれるルールが強烈に働いている場面である。  
`operator<<`の引数が`adl::Data`であることから、コンパイラは「`operator<<`も同様に`adl`名前空間にあるはずだ」として優先的に探しに行ってくれるのである。

さて、これが便利なだけの機能なのであれば特にメモを残したりはしない。
