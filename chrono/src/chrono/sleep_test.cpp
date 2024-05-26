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
  struct test_data
  {
    msecs sleep_time;
    system_clock::time_point start;
    system_clock::time_point end;
  };
  struct test_result
  {
    msecs sleep_time;
  };
  struct test_set
  {
    const std::string name;
    struct test_data i;
    struct test_result expected;
  };

  auto now = time_point_cast<usecs>(system_clock::now());
  struct test_set tt[] = {
    { "待機するケース", { 80ms, now, now + 10ms }, { 70ms } },
    { "待機しないケース", { 80ms, now, now + 100ms }, { -20ms } },
  };
  for (const auto& t : tt) {
    auto actual = flexible_sleep(t.i.sleep_time, t.i.start, t.i.end);
    EXPECT_EQ(t.expected.sleep_time, actual);
  }
}
