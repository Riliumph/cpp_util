#include "calc.hpp"
// 3rd
#include <gtest/gtest.h>
// original
#include "type.hpp"

using namespace std::chrono;
using namespace std::literals::chrono_literals;

TEST(calc_digit, msec)
{
  struct TestData
  {};
  struct TestResult
  {
    int digit;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  struct TestSet tt[] = {
    { "ミリ秒の桁数", {}, { 3 } },
  };
  for (const auto& t : tt) {
    auto actual = calc_digit<msecs>();
    EXPECT_EQ(t.expected.digit, actual);
  }
}

TEST(calc_digit, usec)
{
  struct TestData
  {};
  struct TestResult
  {
    int digit;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  struct TestSet tt[] = {
    { "マイクロ秒の桁数", {}, { 6 } },
  };
  for (const auto& t : tt) {
    auto actual = calc_digit<usecs>();
    EXPECT_EQ(t.expected.digit, actual);
  }
}

TEST(calc_digit, nsec)
{
  struct TestData
  {};
  struct TestResult
  {
    int digit;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  struct TestSet tt[] = {
    { "ナノ秒の桁数", {}, { 9 } },
  };
  for (const auto& t : tt) {
    auto actual = calc_digit<nsecs>();
    EXPECT_EQ(t.expected.digit, actual);
  }
}
