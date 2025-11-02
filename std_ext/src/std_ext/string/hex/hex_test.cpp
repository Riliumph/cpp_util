#include "hex.hpp"
// google test
#include <gtest/gtest.h>
// std_ext
#include "std_ext/bit/bit.hpp"

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
    actual = std_ext::endian::Reverse(actual);
    EXPECT_EQ(t.expected.value, actual);
  }
}
