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
  static T& Get()
  {
    std::call_once(is_initialized, create);
    assert(instance);
    return *instance;
  }

private:
  static void create() { instance = std::make_unique<T>(); }

  static inline std::once_flag is_initialized;
  static inline std::unique_ptr<T> instance;
};

#endif // INCLUDE_SINGLETON_SINGLETON_HPP
