// STL
#include <vector>
// original
#include "singleton/singleton.hpp"

namespace {

std::mutex g_mutex;
std::vector<SingletonFinalizer::FinalizerFunc> g_finalizers;

}

void
SingletonFinalizer::AddFinalizer(FinalizerFunc func)
{
  std::lock_guard<std::mutex> lock(g_mutex);
  g_finalizers.emplace_back(func);
}

void
SingletonFinalizer::Finalize()
{
  std::lock_guard<std::mutex> lock(g_mutex);
  for (const auto& it : g_finalizers) {
    (*it)();
  }
}
