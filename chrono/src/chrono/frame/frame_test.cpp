#include "frame.hpp"
// STL
#include <ctime>
// 3rd
#include <gtest/gtest.h>
// original
#include "chrono/name.hpp"
#include "chrono/operator_io.hpp"

using namespace std::chrono;
using namespace std::literals::chrono_literals;

TEST(time_per_period, fps60_ms)
{
  struct test_data
  {};
  struct test_result
  {
    int msec;
  };
  struct test_set
  {
    const std::string name;
    struct test_data i;
    struct test_result expected;
  };

  struct test_set tt[] = {
    { "60FPS(Hz)は何ミリ秒？", {}, { 16 } },
  };
  for (const auto& t : tt) {
    auto actual = time_per_period<60, msecs>();
    EXPECT_EQ(t.expected.msec, actual);
  }
}

TEST(time_per_period, fps60_us)
{
  struct test_data
  {};
  struct test_result
  {
    int usec;
  };
  struct test_set
  {
    const std::string name;
    struct test_data i;
    struct test_result expected;
  };

  struct test_set tt[] = {
    { "60FPS(Hz)は何マイクロ秒？", {}, { 16666 } },
  };
  for (const auto& t : tt) {
    auto actual = time_per_period<60, usecs>();
    EXPECT_EQ(t.expected.usec, actual);
  }
}
