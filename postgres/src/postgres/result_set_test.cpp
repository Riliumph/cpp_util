#include "postgres/result_set.hpp"
// STL
#include <memory>
#include <string>
#include <typeinfo>
// 3rd
#include <gtest/gtest.h>

using namespace std::literals::string_literals;

namespace postgres {
TEST(ResultSet, Convert2Any)
{
  struct TestData
  {
    std::string query;
  };
  struct TestResult
  {
    db_table result_set;
  };
  struct TestSet
  {
    std::string name;
    struct TestData arg;
    struct TestResult expected;
  };
  struct TestSet tt[] = {
    { "ID1のデータ検証",
      { "select * from student where student_id = 1" },
      { { { { "student_id", 1 }, { "name", "student1"s }, { "age", 17 } } } } },
    { "ID2のデータ検証",
      { "select * from student" },
      { { { { "student_id", 1 }, { "name", "student1"s }, { "age", 17 } },
          { { "student_id", 2 }, { "name", "student2"s }, { "age", 18 } } } } },
  };

  // 事前準備
  auto db_conn = std::make_unique<pqxx::connection>("host=localhost"
                                                    " port=5432"
                                                    " dbname=postgres"
                                                    " user=postgres"
                                                    " password=postgres");
  for (const auto& t : tt) {
    pqxx::work transactor(*db_conn);
    pqxx::result res(transactor.exec(t.arg.query));
    transactor.commit();
    auto actual = ResultSet(res).Map();
    for (std::size_t row = 0; row < t.expected.result_set.size(); ++row) {
      for (const auto& col : t.expected.result_set[row]) {
        auto expected_key = col.first;
        auto expected_val = col.second;
        auto has_key = (actual[row].find(expected_key) != actual[row].end());
        ASSERT_TRUE(has_key);
        auto actual_val = actual[row][expected_key];
        EXPECT_EQ(expected_val, actual_val);
      }
    }
  }
}
}
