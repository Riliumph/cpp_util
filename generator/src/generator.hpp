#ifndef GENERATOR_HPP
#define GENERATOR_HPP
#include <iterator>
class Generator
{
public:
  /// @brief イテレータとして振る舞う内部クラス
  class Iterator
  {
  public:
    // std::next()などで回せるようにするために
    // iteratorのtemplateに添うように型を定義
    using value_type = int;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    Iterator();
    int operator*() const;
    Iterator& operator++();
    Iterator operator++(int);
    bool operator!=(const Iterator&) const;

  private:
    enum class State
    {
      START,
      MIDDLE,
      END
    };
    State state;
  };

  Iterator begin();
  Iterator end();
};

#if 0
  // Pythonのジェネレータの例
  def generator():
      print("Start")
      yield 1  # ここで止まる
      print("Middle")
      yield 2  # ここでまた止まる
      print("End")

  g = generator()
  print(next(g))  # "Start", 1
  print(next(g))  # "Middle", 2
  print(next(g))  # "End"
#endif
#if CXX_VER20
#include <generator>
#include <iostream>

std::generator<int>
generator()
{
  std::cout << "Start" << std::endl;
  co_yield 1; // ここで止まる
  std::cout << "Middle" << std::endl;
  co_yield 2; // ここでまた止まる
  std::cout << "End" << std::endl;
  co_yield 3; // ここでまた止まる
}

int
main()
{
  auto g = generator();

  std::cout << "Next: " << *g.begin() << std::endl; // "Start", 1
  std::cout << "Next: " << *g.begin() << std::endl; // "Middle", 2
  std::cout << "Next: " << *g.begin() << std::endl; // "End", 3

  return 0;
}
#endif

#endif // GENERATOR_HPP
