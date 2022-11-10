#include "postgres/record.hpp"

Record::Record() {}

Record::~Record() {}

std::vector<std::string>
Record::Keys()
{
  std::vector<std::string> keys;
  for (const auto& it : data) {
    keys.emplace_back(it.first);
  }
  return keys;
}

std::vector<std::string>
Record::Values()
{
  std::vector<std::string> vals;
  for (const auto& it : data) {
    vals.emplace_back(it.second);
  }
  return vals;
}

void
Record::Data(std::map<std::string, std::string> data)
{
  this->data = data;
}

std::ostream&
operator<<(std::ostream& os, const Record& l)
{
  for (const auto& it : l.data) {
    os << it.second << "\t";
  }
  return os;
}
