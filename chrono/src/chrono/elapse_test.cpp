#include "elapse.hpp"
// STL
#include <ctime>
// 3rd
#include <gtest/gtest.h>
// original
#include "type.hpp"

using namespace std::chrono;
using namespace std::literals::chrono_literals;

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
