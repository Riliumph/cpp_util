#include "math.hpp"
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

TEST(time_per_period, fps60_ms)
{
  struct TestData
  {};
  struct TestResult
  {
    int msec;
  };
  struct TestSet
  {
    std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  struct TestSet tt[] = {
    { "60FPS(Hz)は何ミリ秒？", {}, { 16 } },
  };
  for (const auto& t : tt) {
    auto actual = time_per_period<60, msecs>();
    EXPECT_EQ(t.expected.msec, actual);
  }
}

TEST(time_per_period, fps60_us)
{
  struct TestData
  {};
  struct TestResult
  {
    int usec;
  };
  struct TestSet
  {
    std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  struct TestSet tt[] = {
    { "60FPS(Hz)は何マイクロ秒？", {}, { 16666 } },
  };
  for (const auto& t : tt) {
    auto actual = time_per_period<60, usecs>();
    EXPECT_EQ(t.expected.usec, actual);
  }
}

TEST(is_unit_time_changed, sec)
{
  struct TestData
  {
    system_clock::time_point start;
    system_clock::time_point end;
  };
  struct TestResult
  {
    bool elapsed;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  auto now = time_point_cast<usecs>(system_clock::now());
  struct TestSet tt[] = {
    { "単位時間(1s)が過ぎているケース", { now, now + 1s }, { true } },
    { "単位時間(1s)が過ぎていないケース", { now, now + 1ms }, { false } },
  };
  for (const auto& t : tt) {
    auto actual = is_unit_time_changed<secs>(t.arg.start, t.arg.end);
    EXPECT_EQ(t.expected.elapsed, actual);
  }
}

TEST(is_unit_time_changed, minute)
{
  struct TestData
  {
    system_clock::time_point start;
    system_clock::time_point end;
  };
  struct TestResult
  {
    bool elapsed;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  auto now = time_point_cast<usecs>(system_clock::now());
  struct TestSet tt[] = {
    { "単位時間(1m)が過ぎているケース", { now, now + 1min }, { true } },
    { "単位時間(1m)が過ぎていないケース", { now, now + 1s }, { false } },
  };
  for (const auto& t : tt) {
    auto actual = is_unit_time_changed<mins>(t.arg.start, t.arg.end);
    EXPECT_EQ(t.expected.elapsed, actual);
  }
}
