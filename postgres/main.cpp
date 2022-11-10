// STL
#include <iostream>
// 3rd
#include <pqxx/pqxx>
// original
#include "postgres/result_set.hpp"

int
main()
{
  std::map<std::string, std::string> data1;
  data1["KEY1"] = "hoge";
  data1["KEY2"] = "fuga";
  std::map<std::string, std::string> data2;
  data2["KEY1"] = "foo";
  data2["KEY2"] = "bar";

  std::vector<ResultSet::Record> records;
  records.emplace_back(data1);
  records.emplace_back(data2);

  ResultSet result_set;
  result_set.Data(records);

  std::cout << result_set << std::endl;
  return 0;
}
