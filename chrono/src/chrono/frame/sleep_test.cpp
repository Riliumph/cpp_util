#include "sleep.hpp"
// STL
#include <ctime>
// 3rd
#include <gtest/gtest.h>
// original
#include "chrono/name.hpp"
#include "chrono/operator_io.hpp"

using namespace std::chrono;
using namespace std::literals::chrono_literals;

TEST(sleep, fps60_msec)
{
  struct TestData
  {
    system_clock::time_point start;
    system_clock::time_point end;
  };
  struct TestResult
  {
    msecs slept_time;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  auto now = time_point_cast<msecs>(system_clock::now());
  struct TestSet tt[] = {
    { "待機するケース", { now, now + 10ms }, { 6ms } },
    { "待機しないケース", { now, now + 20ms }, { -4ms } },
  };
  for (const auto& t : tt) {
    auto actual = sleep<60, msecs>(t.arg.start, t.arg.end);
    EXPECT_EQ(t.expected.slept_time, actual);
  }
}

TEST(sleep, fps30_msec)
{
  struct TestData
  {
    system_clock::time_point start;
    system_clock::time_point end;
  };
  struct TestResult
  {
    msecs slept_time;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  auto now = time_point_cast<msecs>(system_clock::now());
  struct TestSet tt[] = {
    { "待機するケース", { now, now + 10ms }, { 23ms } },
    { "待機しないケース", { now, now + 40ms }, { -7ms } },
  };
  for (const auto& t : tt) {
    auto actual = sleep<30, msecs>(t.arg.start, t.arg.end);
    EXPECT_EQ(t.expected.slept_time, actual);
  }
}

TEST(sleep, fps60_usec)
{
  struct TestData
  {
    system_clock::time_point start;
    system_clock::time_point end;
  };
  struct TestResult
  {
    usecs slept_time;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  auto now = time_point_cast<usecs>(system_clock::now());
  struct TestSet tt[] = {
    { "待機するケース", { now, now + 10ms }, { 6666us } },
    { "待機しないケース", { now, now + 20ms }, { -3334us } },
  };
  for (const auto& t : tt) {
    auto actual = sleep<60, usecs>(t.arg.start, t.arg.end);
    EXPECT_EQ(t.expected.slept_time, actual);
  }
}
