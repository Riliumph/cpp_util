#include "sleep.hpp"
// STL
#include <ctime>
// 3rd
#include <gtest/gtest.h>
// original
#include "name.hpp"
#include "operator_io.hpp"

using namespace std::chrono;
using namespace std::literals::chrono_literals;

TEST(flexible_sleep, msec)
{
  struct TestData
  {
    msecs sleep_time;
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

  auto now = time_point_cast<usecs>(system_clock::now());
  struct TestSet tt[] = {
    { "待機するケース", { 80ms, now, now + 10ms }, { 70ms } },
    { "待機しないケース", { 80ms, now, now + 100ms }, { -20ms } },
  };
  for (const auto& t : tt) {
    auto actual = flexible_sleep(t.arg.sleep_time, t.arg.start, t.arg.end);
    EXPECT_EQ(t.expected.slept_time, actual);
  }
}
