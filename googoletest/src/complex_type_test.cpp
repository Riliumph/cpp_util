// STL
#include <optional>
#include <string>
#include <vector>
// Google Test
#include <gtest/gtest.h>

TEST(complex, type1)
{
  struct Arg
  {
    int arg1;
    int arg2;
  };
  struct TestTable
  {
    std::string test_name;
    struct Arg arg;
    std::optional<std::vector<std::string>> want;
  };
  std::vector<struct TestTable> tt = {
    { "Case1: optional::nullopt check", { 1, 1 }, std::nullopt },
    { "Case2: vector first element", { 2, 2 }, { { "4", "2" } } }
  };

  for (const auto& t : tt) {
    auto actual = t.arg.arg1 + t.arg.arg2;
    if (actual == 2) {
      EXPECT_EQ(t.want, std::nullopt);
    } else {
      EXPECT_EQ(t.want.value()[0], std::to_string(actual));
    }
  }
}

TEST(complex, type2)
{
  struct Arg
  {
    int arg1;
    int arg2;
  };
  struct TestTable
  {
    std::string testname;
    struct Arg arg;
    std::optional<std::vector<std::string>> want;
  };
  std::vector<struct TestTable> tt = {
    { "Case1: optional::nullopt check", { 1, 1 }, std::nullopt },
    { "Case2: vector first element", { 2, 2 }, { { "4", "2" } } }
  };

  for (const auto& t : tt) {
    auto actual = t.arg.arg1 + t.arg.arg2;
    if (actual == 2) {
      EXPECT_EQ(t.want, std::nullopt);
    } else {
      EXPECT_EQ(t.want.value()[0], std::to_string(actual));
    }
  }
}
