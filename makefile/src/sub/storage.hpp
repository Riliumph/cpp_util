#ifndef SUB_STORAGE_HPP
#define SUB_STORAGE_HPP
// STL
#include <ostream>
#include <string>
class Storage
{
public:
  Storage(const std::string&);
  ~Storage() = default;
  friend std::ostream& operator<<(std::ostream& os, const Storage& lv)
  {
    os << lv.data_;
    return os;
  }

private:
  std::string data_;
};

#endif // SUB_STORAGE_HPP
