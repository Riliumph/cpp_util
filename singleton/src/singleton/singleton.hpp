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

  static std::once_flag is_initialized;
  static std::unique_ptr<T> instance;
};

template<typename T>
std::once_flag Singleton<T>::is_initialized;
template<typename T>
std::unique_ptr<T> Singleton<T>::instance;
#endif // INCLUDE_SINGLETON_SINGLETON_HPP
