#include "check.hpp"

namespace std_ext {
namespace endian {
bool
IsLittle()
{
#ifdef __LITTLE_ENDIAN__
  return true;
#elif __BIG_ENDIAN__
  return false;
#else
  int t = 1;
  return *reinterpret_cast<char*>(&t) == 1;
#endif
}
} // namespace endian
} // namespace std_ext
