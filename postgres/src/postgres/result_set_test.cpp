// #ifdef DB_TEST
#include "postgres/result_set.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <typeinfo>
namespace postgres {
TEST(ResultSet, Convert2Any)
{
  // SQL
  auto db_conn = std::make_unique<pqxx::connection>("host=localhost"
                                                    " port=5432"
                                                    " dbname=postgres"
                                                    " user=postgres"
                                                    " password=postgres");
  pqxx::work transactor(*db_conn);
  auto query = "select * from student";
  pqxx::result res(transactor.exec(query));
  transactor.commit();
  // Test
  auto result_set = ResultSet(res);
  auto actual = result_set.Map();
  db_table tt = {
    // std::vector = 行データ
    { // std::map = 列データ
      { "id", 1 },
      { "name", std::string("student1") },
      { "age", 17 } },
    { // std::map
      { "id", 2 },
      { "name", std::string("student2") },
      { "age", 18 } },
  };

  for (std::size_t i = 0; i < tt.size(); ++i) {
    auto& actual_record = actual[i];
    for (const auto& t : tt[i]) {
      auto expected_key = t.first;
      auto expected_val = t.second;
      auto has_key = (actual_record.find(expected_key) != actual_record.end());
      // テスト
      ASSERT_TRUE(has_key);
      auto actual_val = actual_record[expected_key];
      EXPECT_EQ(expected_val, actual_val);
    }
  }
}
}
// #endif
