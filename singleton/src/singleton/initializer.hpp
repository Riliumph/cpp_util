#ifndef INCLUDE_SINGLETON_INITIALIZER_HPP
#define INCLUDE_SINGLETON_INITIALIZER_HPP
// STL
#include <cassert>
#include <memory>
#include <mutex>
// singleton
#include "finalizer.hpp"

namespace singleton {
/// @brief シングルトンを作るクラス
/// @tparam T 作りたい型
template<typename T>
class Initializer final
{
public:
  /// @brief シングルトンインスタンスを取得する
  /// @tparam ...Args 引数の型群
  /// @param ...args 引数群
  /// @return シングルトンインスタンスへの参照
  template<typename... Args>
  static T& Get(Args&&... args)
  {
    std::call_once(
      is_initialized, create<Args...>, std::forward<Args>(args)...);
    assert(instance);
    return *instance;
  }

private:
  /// @brief インスタンスを生成する
  /// @tparam ...Args 引数の型群
  /// @param ...args 引数群
  template<typename... Args>
  static void create(Args&&... args)
  {
    instance = std::make_unique<T>(std::forward<Args>(args)...);
    Finalizer::Add([&]() { instance.reset(nullptr); });
  }

  static inline std::once_flag is_initialized;
  static inline std::unique_ptr<T> instance;
};
}
#endif // INCLUDE_SINGLETON_INITIALIZER_HPP
