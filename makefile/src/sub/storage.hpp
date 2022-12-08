#ifndef SUB_STORAGE_HPP
#define SUB_STORAGE_HPP
// STL
#include <ostream>
#include <string>
class Storage
{
public:
  Storage() = default;
  Storage(std::string);
  ~Storage() = default;
  friend std::ostream& operator<<(std::ostream&, const Storage&);
  std::string Data();
  void Data(std::string);

private:
  std::string data_;
};

std::ostream&
operator<<(std::ostream&, const Storage&);
#endif // SUB_STORAGE_HPP
