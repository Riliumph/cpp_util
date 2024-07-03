#ifndef INCLUDE_SINGLETON_SINGLETON_HPP
#define INCLUDE_SINGLETON_SINGLETON_HPP
// STL
#include <cassert>
#include <memory>
#include <mutex>
template<typename T>
class Singleton final
{
public:
  template<typename... Args>
  static T& Get(Args&&... args)
  {
    std::call_once(
      is_initialized, create<Args...>, std::forward<Args>(args)...);
    assert(instance);
    return *instance;
  }

private:
  template<typename... Args>
  static void create(Args&&... args)
  {
    instance = std::make_unique<T>(std::forward<Args>(args)...);
  }

  static inline std::once_flag is_initialized;
  static inline std::unique_ptr<T> instance;
};

#endif // INCLUDE_SINGLETON_SINGLETON_HPP
