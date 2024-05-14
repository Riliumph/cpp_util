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

TEST(fps_sleep, fps60_msec)
{
  struct test_data
  {
    system_clock::time_point start;
    system_clock::time_point end;
  };
  struct test_result
  {
    msecs expected;
  };
  struct test_set
  {
    const std::string name;
    struct test_data i;
    struct test_result o;
  };

  auto now = time_point_cast<msecs>(system_clock::now());
  struct test_set tt[] = {
    { "待機するケース", { now, now + 10ms }, { 6ms } },
    { "待機しないケース", { now, now + 20ms }, { -4ms } },
  };
  for (const auto& t : tt) {
    auto actual = fps_sleep<60, msecs>(t.i.start, t.i.end);
    EXPECT_EQ(t.o.expected, actual);
  }
}

TEST(fps_sleep, fps30_msec)
{
  struct test_data
  {
    system_clock::time_point start;
    system_clock::time_point end;
  };
  struct test_result
  {
    msecs expected;
  };
  struct test_set
  {
    const std::string name;
    struct test_data i;
    struct test_result o;
  };

  auto now = time_point_cast<msecs>(system_clock::now());
  struct test_set tt[] = {
    { "待機するケース", { now, now + 10ms }, { 23ms } },
    { "待機しないケース", { now, now + 40ms }, { -7ms } },
  };
  for (const auto& t : tt) {
    auto actual = fps_sleep<30, msecs>(t.i.start, t.i.end);
    EXPECT_EQ(t.o.expected, actual);
  }
}

TEST(fps_sleep, fps60_usec)
{
  struct test_data
  {
    system_clock::time_point start;
    system_clock::time_point end;
  };
  struct test_result
  {
    usecs expected;
  };
  struct test_set
  {
    const std::string name;
    struct test_data i;
    struct test_result o;
  };

  auto now = time_point_cast<usecs>(system_clock::now());
  struct test_set tt[] = {
    { "待機するケース", { now, now + 10ms }, { 6666us } },
    { "待機しないケース", { now, now + 20ms }, { -3334us } },
  };
  for (const auto& t : tt) {
    auto actual = fps_sleep<60, usecs>(t.i.start, t.i.end);
    EXPECT_EQ(t.o.expected, actual);
  }
}
