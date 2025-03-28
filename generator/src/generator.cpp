#include "generator.hpp"
#include <iostream>
#include <stdexcept> // for std::out_of_range
/// @brief コンストラクタ
Generator::Iterator::Iterator()
  : state(State::START)
{
}

/// @brief 現在の要素を取得する。
/// @details PythonのGenerator関数の実装そのもの。
/// @return Pythonのyield値と同じ値を返す
int
Generator::Iterator::operator*() const
{
#if 0
  // Pythonのジェネレータの例
  def generator():
      print("Start")
      yield 1  # ここで止まる
      print("Middle")
      yield 2  # ここでまた止まる
      print("End")
      yield 3  # 終わり
#endif
  switch (state) {
    case State::START:
      std::cout << "Start" << std::endl;
      return 1; // yield 1と同じ
    case State::MIDDLE:
      std::cout << "Middle" << std::endl;
      return 2; // yield 2と同じ
    case State::END:
      std::cout << "End" << std::endl;
      return 3; // yield 3と同じ
    default:
      throw std::out_of_range("No more elements");
  }
}

/// @brief 次の要素を返す（前置インクリメント）
/// @details インスタンスの状態を進めて、進んだインスタンスを返す
/// @return 進んだインスタンス
Generator::Iterator&
Generator::Iterator::operator++()
{
  switch (state) {
    case State::START:
      state = State::MIDDLE;
      std::cout << "Start -> MIDDLE" << std::endl;
      break;
    case State::MIDDLE:
      state = State::END;
      std::cout << "MIDDLE -> END" << std::endl;
      break;
    case State::END:
      std::cout << "END -> END" << std::endl;
      break;
  }
  return *this;
}

/// @brief 今の要素を返して、状態を進める（後置インクリメント）
/// @return
Generator::Iterator
Generator::Iterator::operator++(int)
{
  Iterator tmp = *this; // 今の状態を保存
  ++(*this);            // 状態を進める
  return tmp;           // 保存した過去の状態を返す
}

bool
Generator::Iterator::operator!=(const Iterator& other) const
{
  return state != other.state;
}

// Generatorの実装

Generator::Iterator
Generator::begin()
{
  return Iterator();
}

Generator::Iterator
Generator::end()
{
  Iterator it;
  // `end` 状態に設定
  ++it;
  return it;
}
