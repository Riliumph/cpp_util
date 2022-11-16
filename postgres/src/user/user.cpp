// original
#include "user/user.hpp"

namespace entity {
#ifndef ACCESSOR
void
User::ID(const int id)
{
  this->id = id;
}

int
User::ID()
{
  return id;
}

void
User::Name(const std::string& name)
{
  this->name = name;
}

std::string
User::Name()
{
  return name;
}

void
User::Age(const int age)
{
  this->age = age;
}

int
User::Age()
{
  return age;
}
#endif // ACCESSOR
std::ostream&
operator<<(std::ostream& os, const User& lv)
{
  os << "[" << lv.id << "]" << lv.name;
  return os;
}
} // entity
