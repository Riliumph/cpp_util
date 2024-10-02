#include "diff.hpp"
// GoogleTest
#include <gtest/gtest.h>

TEST(set_difference_by_regex, Test)
{
  struct TestData
  {
    std::set<std::string> required;
    std::set<std::string> provided;
  };
  struct TestResult
  {
    std::set<std::string> missing_key;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  struct TestSet tt[] = {
    {
      "完全一位置のケース",
      {
        { R"(foo)", R"(bar)" },
        { "foo", "bar" },
      },
      { {} },
    },
    {
      "一部に含まれるケース",
      {
        { R"(foo.*)", R"(.*bar)" },
        { "foobar" },
      },
      { {} },
    },
    {
      "数値を検出するケース",
      {
        { R"(.*\d*)" },
        { "foo123" },
      },
      { {} },
    },
    {
      "先頭に数値を検出するケース",
      {
        { R"(\d*)" },
        { "foo", "123" },
      },
      { {} },
    },
    {
      "含まれないケース",
      {
        { R"(foo)" },
        { "hoge" },
      },
      { { R"(foo)" } },
    },
  };
  for (const auto& t : tt) {
    auto actual = set_difference_by_regex(t.arg.required, t.arg.provided);
    EXPECT_EQ(t.expected.missing_key, actual);
  }
}
