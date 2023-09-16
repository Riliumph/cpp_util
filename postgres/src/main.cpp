// STL
#include <algorithm>
#include <iostream>
#include <memory>
// 3rd
#include <pqxx/pqxx>
// original
#include "any/py_any.hpp"
#include "postgres/result_set.hpp"

int
main()
{
  try {
    auto db_conn = std::make_unique<pqxx::connection>("host=localhost"
                                                      " port=5432"
                                                      " dbname=postgres"
                                                      " user=postgres"
                                                      " password=postgres");
    pqxx::work transactor(*db_conn);
    auto query = "select * from student";
    pqxx::result res(transactor.exec(query));
    transactor.commit();

    std::cout << "map like python" << std::endl;
    auto users = py::Convert2Any(res);
    std::cout << users << std::endl;

  } catch (const pqxx::sql_error& e) {
    std::cerr << e.what() << " : SQL->" << e.query() << std::endl;
  } catch (const pqxx::usage_error& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
