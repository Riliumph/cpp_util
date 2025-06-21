#include "ecast.hpp"

#include <gtest/gtest.h>

namespace std_ext {
TEST(ecast, cast)
{
  enum struct COLOR
  {
    RED,
    GREEN,
    BLUE,
    QTY
  };
  struct TestData
  {
    enum COLOR color;
  };
  struct TestResult
  {
    int expected_value;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  struct TestSet tt[] = {
    { "REDの変換", { COLOR::RED }, { 0 } },
    { "GREENの変換", { COLOR::GREEN }, { 1 } },
    { "BLUEの変換", { COLOR::BLUE }, { 2 } },
    { "QTYの変換", { COLOR::QTY }, { 3 } },
  };
  for (const auto& t : tt) {
    auto actual = e_cast(t.arg.color);
    EXPECT_EQ(t.expected.expected_value, actual);
  }
}
} // namespace std_ext
