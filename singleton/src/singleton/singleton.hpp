#ifndef INCLUDE_SINGLETON_SINGLETON_HPP
#define INCLUDE_SINGLETON_SINGLETON_HPP
// STL
#include <cassert>
#include <mutex>
// original
#include "finalizer.hpp"

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
  static void create()
  {
    instance = new T;
    singleton::Finalizer::Add(&Singleton<T>::destroy);
  }

  static void destroy()
  {
    delete instance;
    instance = nullptr;
  }

  static std::once_flag is_initialized;
  static T* instance;
};

template<typename T>
std::once_flag Singleton<T>::is_initialized;
template<typename T>
T* Singleton<T>::instance = nullptr;
#endif // INCLUDE_SINGLETON_SINGLETON_HPP
