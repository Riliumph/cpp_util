#include "hex.hpp"
// google test
#include <gtest/gtest.h>

TEST(HexTo, int64)
{
  struct TestArg
  {
    std::string s;
  };

  struct TestResult
  {
    int64_t value;
  };

  struct TestSet
  {
    std::string name;
    TestArg i;
    TestResult expected;
  };

  struct TestSet tt[] = {
    { "正常系", { "0000000000000001" }, { 1 } },
    { "正常系", { "000000000000000a" }, { 10 } },
  };

  for (const auto& t : tt) {
    auto actual = HexTo<int64_t>(t.i.s);
    EXPECT_EQ(t.expected.value, actual);
  }
}
