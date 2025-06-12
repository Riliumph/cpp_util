#ifndef STD_EXT_CHRONO_CORE_CONSTRAINT_HPP
#define STD_EXT_CHRONO_CORE_CONSTRAINT_HPP
// STL
#include <chrono>
#include <type_traits>

namespace std_ext {
namespace chrono {
template<typename T>
using is_chrono_duration =
  std::is_base_of<std::chrono::duration<typename T::rep, typename T::period>,
                  T>;

template<typename T>
using enable_if_chrono_duration_t =
  std::enable_if_t<is_chrono_duration<T>::value>;
} // namespace chrono
} // namespace std_ext
#endif // STD_EXT_CHRONO_CORE_CONSTRAINT_HPP
