#ifndef RESULT_SET_HPP
#define RESULT_SET_HPP
// STL
#include <map>
#include <ostream>
#include <string>
#include <vector>
// original
// #include "postgres/record.hpp"

class ResultSet
{
public:
  using Record = std::map<std::string, std::string>;
  ResultSet();
  ~ResultSet();
  friend std::ostream& operator<<(std::ostream&, const ResultSet&);

  void Data(std::vector<Record> data);
  std::vector<Record> Data();

private:
  std::vector<Record> data;
};

std::ostream&
operator<<(std::ostream&, const ResultSet&);

#endif // RESULT_SET_HPP
