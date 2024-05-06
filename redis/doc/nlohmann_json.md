# nlohmann::json

## iteratorの使い方

参考

- [nlohmann::basic_json::items](https://json.nlohmann.me/api/basic_json/items/)

### jsonは内部でどんな展開が行われるか？

```cpp
using json = nlohmann::json;
json val = { { "username", "hoge" },
            { "e-mail", "hoge@mail.com" },
            { "age", 17 } };
for (const auto& v : val) {
    std::cout << "elem: " << v << std::endl
              << "type: " << typeid(v).name() << std::endl
              << "dump: " << v.dump() << std::endl
              << "dump type: " << typeid(v.dump()).name() << std::endl
              << std::endl;
}
```

`cout`の出力結果は名前マングリングされているので読めたものではないので、vscodeのデバッガーから調べてみる。

- 1回目：`nlohmann::json_abi_v3_11_2::detail::value_t::number_integer`
- 2回目：`nlohmann::json_abi_v3_11_2::detail::value_t::string`
- 3回目：`nlohmann::json_abi_v3_11_2::detail::value_t::string`

なるほど。型が変わるのか。

> for文の型としては、`value_t`で問題ない。

forに突入した段階で期待したデータは`{"username", "hoge"}`である。  
しかし、取れたのは`{"hoge"}`の部分だけである。

キー名が必要ない場合は、このforで十分だろう。

### items()メソッドは内部でどんな展開が行われるか？

```cpp
using json = nlohmann::json;
json val = { { "username", "hoge" },
            { "e-mail", "hoge@mail.com" },
            { "age", 17 } };
for (const auto& v : val.items()) {
std::cout << "elem: " << v << std::endl
          << "type: " << typeid(v).name() << std::endl
          << "key: " << v.key() << std::endl
          << "val: " << v.value() << std::endl
          << std::endl;
}
```

`items()`メソッドの実装は以下になっている。

```cpp
using iterator = iter_impl<basic_json>;
iteration_proxy<iterator> items() noexcept
{
    return iteration_proxy<iterator>(*this);
}
```

ちゃんと、イテレータを返しているようだ。  
usingでも分かるように、この`iterator`は内部では`basic_json`であることから、jsonのもっとも基本的なクラスである。

つまり、キー情報も取れるし、バリュー情報も当然とれる。  
次へのポインタもしっかり知ってる。

基本はこっちを使わないと欲しい情報が取れないことが多い。
