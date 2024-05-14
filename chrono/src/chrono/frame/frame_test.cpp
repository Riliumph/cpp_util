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

TEST(time_per_frame, fps60_ms)
{
  struct test_data
  {};
  struct test_result
  {
    int expected;
  };
  struct test_set
  {
    const std::string name;
    struct test_data i;
    struct test_result o;
  };

  struct test_set tt[] = {
    { "60FPSのミリ秒計算", {}, { 16 } },
  };
  for (const auto& t : tt) {
    auto actual = time_per_frame<60, msecs>();
    EXPECT_EQ(t.o.expected, actual);
  }
}

TEST(time_per_frame, fps60_us)
{
  struct test_data
  {};
  struct test_result
  {
    int expected;
  };
  struct test_set
  {
    const std::string name;
    struct test_data i;
    struct test_result o;
  };

  struct test_set tt[] = {
    { "60FPSのマイクロ秒計算", {}, { 16666 } },
  };
  for (const auto& t : tt) {
    auto actual = time_per_frame<60, usecs>();
    EXPECT_EQ(t.o.expected, actual);
  }
}
