#include "find.hpp"
// 3rd
#include <gtest/gtest.h>

namespace std_ext {
namespace range {
TEST(range_package, find)
{
  struct TestData
  {
    std::vector<int> values;
    std::function<bool(int)> predicate;
  };
  struct TestResult
  {
    std::optional<int> value;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  struct TestSet tt[] = {
    {
      "偶数を探す",
      { { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, [&](auto v) { return v % 2 == 0; } },
      { 2 },
    },
    {
      "奇数を探す",
      { { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, [&](auto v) { return v % 2 == 1; } },
      { 1 },
    },
    {
      "見つからない",
      { { 1, 3, 5, 7, 9 }, [&](auto v) { return v % 2 == 0; } },
      { std::nullopt },

    }
  };
  for (const auto& t : tt) {
    auto actual = std_ext::range::find_if(t.arg.values, t.arg.predicate);
    if (t.expected.value == std::nullopt) {
      EXPECT_EQ(t.arg.values.end(), actual);
    } else {
      EXPECT_EQ(*t.expected.value, *actual);
    }
  }
}
} // namespace range
} // namespace std_ext
