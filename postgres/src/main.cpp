// STL
#include <algorithm>
#include <iostream>
#include <memory>
// 3rd
#include <pqxx/pqxx>
// original
#include "postgres/result_set.hpp"
#include "user/py_user.hpp"

int
main()
{
  try {
    auto db_conn = std::make_unique<pqxx::connection>("host=db_server"
                                                      " port=5432"
                                                      " dbname=sample_db"
                                                      " user=postgres"
                                                      " password=postgres");
    pqxx::work transactor(*db_conn);
    auto query = "select * from student";
    pqxx::result res(transactor.exec(query));
    transactor.commit();

    std::cout << "map like python" << std::endl;
    auto users = py::Convert2User(res);
    std::cout << users << std::endl;

  } catch (const pqxx::sql_error& e) {
    std::cerr << e.what() << " : SQL->" << e.query() << std::endl;
  } catch (const pqxx::usage_error& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
