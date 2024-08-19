#include "hex.hpp"
// google test
#include <gtest/gtest.h>
// endian
#include "endian.hpp"

TEST(HexTo, big_endian_int64)
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
    { "正常系", { "0100000000000000" }, { 1 } },
    { "正常系", { "0a00000000000000" }, { 10 } },
  };

  for (const auto& t : tt) {
    auto actual = HexTo<int64_t>(t.i.s);
    actual = endian::Reverse(actual);
    EXPECT_EQ(t.expected.value, actual);
  }
}
