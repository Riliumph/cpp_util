// STL
#include <optional>
#include <string>
#include <vector>
// Google Test
#include <gtest/gtest.h>

TEST(simple, simple)
{
  struct TestData
  {
    int arg1;
    int arg2;
  };
  struct TestResult
  {
    int want;
  };
  struct TestSet
  {
    std::string test_name;
    struct TestData arg;
    struct TestResult expected;
  };
  struct TestSet tt[] = {
    { "Case1", { 1, 1 }, 2 },
    { "Case2", { 1, 3 }, 4 },
  };
  for (const auto& t : tt) {
    auto actual = t.arg.arg1 + t.arg.arg2;
    EXPECT_EQ(t.expected.want, actual);
  }
}

TEST(simple, exception)
{
  struct TestData
  {
    bool can_throw_exception;
  };
  struct TestResult
  {
    int want;
  };
  struct TestSet
  {
    std::string test_name;
    struct TestData arg;
    struct TestResult expected;
  };

  std::vector<struct TestSet> tests = {
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
    EXPECT_EQ(t.expected.want, 1);
  }
}
