#ifndef INCLUDE_SINGLETON_FINALIZER_HPP
#define INCLUDE_SINGLETON_FINALIZER_HPP
// STL
#include <cassert>
#include <functional>
#include <mutex>
#include <vector>

class Finalizer final
{
public:
  static void Add(std::function<void()>&& func)
  {
    finalizers.emplace_back(std::forward<decltype(func)>(func));
  }
  static void Finalize() noexcept
  {
    std::for_each(
      finalizers.crbegin(), finalizers.crend(), [](auto&& fn) { fn(); });
    finalizers.clear();
  }

private:
  Finalizer() = delete;
  static inline std::vector<std::function<void()>> finalizers;
};

#endif // INCLUDE_SINGLETON_FINALIZER_HPP
