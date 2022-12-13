#ifndef INCLUDE_SINGLETON_FINALIZER_HPP
#define INCLUDE_SINGLETON_FINALIZER_HPP

namespace singleton {
class Finalizer
{
public:
  using Func = void (*)();
  static void Add(Func func);
  static void Finalize();
};
} // namespace singleton
#endif // INCLUDE_SINGLETON_FINALIZER_HPP
