// STL
#include <mutex>
#include <vector>
// original
#include "finalizer.hpp"

namespace {
std::mutex g_mutex;
std::vector<singleton::Finalizer::Func> g_finalizers;
}

namespace singleton {
void
Finalizer::Add(Finalizer::Func func)
{
  std::lock_guard<std::mutex> lock(g_mutex);
  g_finalizers.emplace_back(func);
}

void
Finalizer::Finalize()
{
  std::lock_guard<std::mutex> lock(g_mutex);
  for (const auto& it : g_finalizers) {
    (*it)();
  }
}
} // namespace singleton
