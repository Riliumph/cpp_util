// STL
#include <optional>
#include <string>
#include <vector>
// Google Test
#include <gtest/gtest.h>

TEST(simple, simple)
{
  struct Arg
  {
    int arg1;
    int arg2;
  };
  struct Test
  {
    std::string test_name;
    struct Arg arg;
    int want;
  };
  std::vector<struct Test> tests = {
    { "Case1", { 1, 1 }, 2 },
    { "Case2", { 1, 3 }, 4 },
  };
  for (const auto& t : tests) {
    auto actual = t.arg.arg1 + t.arg.arg2;
    EXPECT_EQ(t.want, actual);
  }
}

TEST(simple, exception)
{
  struct Arg
  {
    bool can_throw_exception;
  };
  struct Test
  {
    std::string test_name;
    struct Arg arg;
    int want;
  };
  std::vector<struct Test> tests = {
    { "Case1", { false }, 1 },
    { "Case2", { true }, 1 },
  };
  for (const auto& t : tests) {
    try {
      if (t.arg.can_throw_exception) {
        throw std::runtime_error("runtime error message");
      }
    } catch (std::exception& e) {

      FAIL() << "exception: " << e.what();
    }
    EXPECT_EQ(t.want, 1);
  }
}
