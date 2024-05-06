
#include <iostream>
#include <unordered_map>
#include <vector>
template<typename T>
std::ostream&
operator<<(std::ostream& os, const std::vector<T>& vs)
{
  if (vs.empty()) {
    os << "[]";
    return os;
  }
  os << "[" << vs.front();
  for (auto it = ++vs.begin(); it != vs.end(); ++it) {
    os << ", " << *it;
  }
  os << "]";
  return os;
}

// template<typename T, typename U>
// std::ostream&
// operator<<(std::ostream& os, const std::unordered_map<T, U>& lv)
// {
//   for (const auto& v : lv) {
//     os << '{' << v.first << ',' << v.second << "}, ";
//   }
//   return os;
// }
