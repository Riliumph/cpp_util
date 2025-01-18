// STL
#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
// 3rd
#include <pqxx/pqxx>
// original
#include "chrono.hpp"
#include "postgres.hpp"

int
main()
{
  auto now = std::chrono::system_clock::now();
  std::cout << now << std::endl;
  try {
    auto db_conn = std::make_unique<pqxx::connection>("host=localhost"
                                                      " port=5432"
                                                      " dbname=postgres"
                                                      " user=postgres"
                                                      " password=postgres");
    pqxx::work transactor(*db_conn);
    // 警告は次の方法で個別に消すことも可能
    // NOLINTNEXTLINE(readability-qualified-auto)
    auto query = "select * from student";
    pqxx::result res(transactor.exec(query));
    transactor.commit();

    std::cout << "map like python" << std::endl;
    auto users = postgres::ResultSet(res);
    std::cout << users << std::endl;

  } catch (const pqxx::sql_error& e) {
    std::cerr << e.what() << " : SQL->" << e.query() << std::endl;
  } catch (const pqxx::usage_error& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
