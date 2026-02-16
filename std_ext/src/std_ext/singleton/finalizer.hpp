#ifndef INCLUDED_STD_EXT_SINGLETON_FINALIZER_HPP
#define INCLUDED_STD_EXT_SINGLETON_FINALIZER_HPP
// STL
#include <cassert>
#include <functional>
#include <mutex>
#include <vector>

namespace std_ext {
namespace singleton {
class Finalizer final
{
public:
  using finalizer_t = std::function<void()>;

  /// @brief Finalizerに終了処理関数を追加する
  /// @param func 終了処理関数
  static void Add(finalizer_t&& func)
  {
    finalizers.emplace_back(std::forward<finalizer_t>(func));
  }

  /// @brief Finalizerを実行する。
  static void Finalize() noexcept
  {
    std::for_each(
      finalizers.crbegin(), finalizers.crend(), [](auto&& fn) { fn(); });
    finalizers.clear();
  }

private:
  Finalizer() = delete;
  static inline std::vector<finalizer_t> finalizers;
};
} // namespace singleton
} // namespace std_ext
#endif // INCLUDED_STD_EXT_SINGLETON_FINALIZER_HPP
