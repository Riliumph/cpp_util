#ifndef RECORD_HPP
#define RECORD_HPP
// STL
#include <map>
#include <ostream>
#include <string>
#include <vector>

class ResultSet;

class Record
{
  friend ResultSet;

public:
  Record();
  ~Record();
  friend std::ostream& operator<<(std::ostream&, const Record&);

  std::vector<std::string> Keys();
  std::string Key(int);
  std::vector<std::string> Values();
  std::string Value(int);
  void Data(std::map<std::string, std::string>);

private:
  std::map<std::string, std::string> data;
};

std::ostream&
operator<<(std::ostream&, const Record&);
#endif // RECORD_HPP
