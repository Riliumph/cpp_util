// original
#include "sub/storage.hpp"

#ifndef CONSTRUCTOR
Storage::Storage(std::string data)
  : data_(data)
{
}
#endif // CONSTRUCTOR
#ifndef ACCESSOR
std::string
Storage::Data()
{
  return data_;
}

void
Storage::Data(std::string data)
{
  data_ = data;
}
#endif // ACCESSOR

#ifndef OPERATOR_OVERLOAD
std::ostream&
operator<<(std::ostream& os, const Storage& lv)
{
  os << lv.data_;
  return os;
}
#endif // OPERATOR_OVERLOAD
