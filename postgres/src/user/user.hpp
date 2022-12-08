#ifndef USER_HPP
#define USER_HPP
// STL
#include <ostream>

namespace entity {
class User
{
public:
  User() = default;
  ~User() = default;
  friend std::ostream& operator<<(std::ostream&, const User&);

  void ID(const int);
  int ID();
  void Name(const std::string&);
  std::string Name();
  void Age(const int);
  int Age();

private:
  int id;
  std::string name;
  double age;
};
std::ostream&
operator<<(std::ostream&, const User&);
} // entity
#endif // USER_HPP
