#include "postgres/result_set.hpp"

ResultSet::ResultSet() {}

ResultSet::~ResultSet() {}

void
ResultSet::Data(std::vector<Record> data)
{
  this->data = data;
}

std::vector<ResultSet::Record>
ResultSet::Data()
{
  return this->data;
}

std::ostream&
operator<<(std::ostream& os, const ResultSet& r)
{
  if (r.data.size() == 0) {
    return os;
  }
  // show header
  for (const auto& key : r.data[0]) {
    os << key.first << "\t";
  }
  os << std::endl;
  // show data
  for (const auto& record : r.data) {
    for (auto i : record) {
      os << i.second << "\t";
    }
    os << std::endl;
  }
  return os;
}
