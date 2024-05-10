// STL
#include <ostream>
// 3rd
#include <nlohmann/json.hpp>

class Data
{
public:
  Data() = default;
  Data(const std::string& name, const std::string& email, const int age)
  {
    this->name = name;
    this->email = email;
    this->age = age;
  }
  ~Data(){};

  void Name(const std::string& v) { name = v; };
  void Age(const int v) { age = v; };
  void Email(const std::string& v) { email = v; };

  friend std::ostream& operator<<(std::ostream& os, const Data& lv)
  {
    os << "Name: " << lv.name << std::endl;
    os << "Email: " << lv.email << std::endl;
    os << "Age: " << lv.age << std::endl;
  };

  friend void from_json(const nlohmann::json& j, Data& lv)
  {
    j.at("name").get_to(lv.name);
    j.at("email").get_to(lv.email);
    j.at("age").get_to(lv.age);
  };

  friend void to_json(nlohmann::json& j, const Data& lv)
  {
    j = nlohmann::json{ { "name", lv.name },
                        { "email", lv.email },
                        { "age", lv.age } };
  };

private:
  std::string name;
  std::string email;
  int age;
};
