// STL
#include <iostream>
// original
#include "singleton.hpp"

class Data
{
public:
  int data;
};

void
routine();
void
ref_routine();

int
main(void)
{
  auto only_one = singleton::Factory<Data>::Get();
  // 通常アクセスでデータを変更する。
  std::cout << "store local & change data by normal access." << std::endl;
  std::cout << "before: " << only_one.data << std::endl;
  routine();
  std::cout << "after: " << only_one.data << std::endl;
  std::cout << "new instance: " << singleton::Factory<Data>::Get().data
            << std::endl;

  // 参照アクセスでデータを変更するが、依然に取得した一時変数には反映されない。
  std::cout << "store local & change data by reference access." << std::endl;
  std::cout << "before: " << only_one.data << std::endl;
  ref_routine();
  std::cout << "after: " << only_one.data << std::endl;
  std::cout << "new instance: " << singleton::Factory<Data>::Get().data
            << std::endl;

  // 一時変数を経由すると値が追従できないので、都度アクセスするのが良い。
  std::cout << "do not store in a local variable." << std::endl;
  std::cout << "you had better get instance each time." << std::endl;
  return 0;
}

/// @brief 通常の取得では値を変えることはできない
void
routine()
{
  auto only_one = singleton::Factory<Data>::Get();
  only_one.data = 1000;
}

/// @brief 参照の取得では値を変えることができる
void
ref_routine()
{
  auto& ref_only_one = singleton::Factory<Data>::Get();
  ref_only_one.data = 2000;
}
