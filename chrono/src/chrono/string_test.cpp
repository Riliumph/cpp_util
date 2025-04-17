#include "string.hpp"
// 3rd
#include <gtest/gtest.h>
// original
#include "type.hpp"

using namespace std::chrono;
using namespace std::literals::chrono_literals;

TEST(to_iso8601, min)
{
  struct TestData
  {
    int h;
    int m;
  };
  struct TestResult
  {
    std::string str;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  struct TestSet tt[] = {
    { "1970-01-01T01:02:00", { 1, 2 }, { "1970-01-01T01:02:00" } },
  };
  for (const auto& t : tt) {
    auto tp = std::chrono::system_clock::time_point{};
    tp = tp + hours(t.arg.h) + mins(t.arg.m);
    auto actual = to_iso8601<mins>(tp);
    EXPECT_EQ(t.expected.str, actual);
  }
}

TEST(to_iso8601, sec)
{
  struct TestData
  {
    // C++20以前はstd::chrono::system_clock::time_pointを直接作ることができない
    int h;
    int m;
    int s;
  };
  struct TestResult
  {
    std::string str;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  struct TestSet tt[] = {
    { "1970-01-01T01:02:03", { 1, 2, 3 }, { "1970-01-01T01:02:03" } },
  };
  for (const auto& t : tt) {
    auto tp = std::chrono::system_clock::time_point{};
    tp = tp + hours(t.arg.h) + mins(t.arg.m) + secs(t.arg.s);
    auto actual = to_iso8601<secs>(tp);
    EXPECT_EQ(t.expected.str, actual);
  }
}

TEST(to_iso8601, msec)
{
  struct TestData
  {
    // C++20以前はstd::chrono::system_clock::time_pointを直接作ることができない
    int h;
    int m;
    int s;
    int ms;
  };
  struct TestResult
  {
    std::string str;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  struct TestSet tt[] = {
    { "1970-01-01T01:02:03.04", { 1, 2, 3, 4 }, { "1970-01-01T01:02:03.004" } },
  };
  for (const auto& t : tt) {
    auto tp = std::chrono::system_clock::time_point{};
    tp = tp + hours(t.arg.h) + mins(t.arg.m) + secs(t.arg.s) + msecs(t.arg.ms);
    auto actual = to_iso8601<msecs>(tp);
    EXPECT_EQ(t.expected.str, actual);
  }
}

TEST(to_iso8601, usec)
{
  struct TestData
  {
    // C++20以前はstd::chrono::system_clock::time_pointを直接作ることができない
    int h;
    int m;
    int s;
    int us;
  };
  struct TestResult
  {
    std::string str;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  struct TestSet tt[] = {
    { "1970-01-01T01:02:03.04",
      { 1, 2, 3, 4 },
      { "1970-01-01T01:02:03.000004" } },
  };
  for (const auto& t : tt) {
    auto tp = std::chrono::system_clock::time_point{};
    tp = tp + hours(t.arg.h) + mins(t.arg.m) + secs(t.arg.s) + usecs(t.arg.us);
    auto actual = to_iso8601<usecs>(tp);
    EXPECT_EQ(t.expected.str, actual);
  }
}

TEST(to_iso8601, nsec)
{
  struct TestData
  {
    // C++20以前はstd::chrono::system_clock::time_pointを直接作ることができない
    int h;
    int m;
    int s;
    int ns;
  };
  struct TestResult
  {
    std::string str;
  };
  struct TestSet
  {
    const std::string name;
    struct TestData arg;
    struct TestResult expected;
  };

  struct TestSet tt[] = {
    { "1970-01-01T01:02:03.04",
      { 1, 2, 3, 4 },
      { "1970-01-01T01:02:03.000000004" } },
  };
  for (const auto& t : tt) {
    auto tp = std::chrono::system_clock::time_point{};
    tp = tp + hours(t.arg.h) + mins(t.arg.m) + secs(t.arg.s) + nsecs(t.arg.ns);
    auto actual = to_iso8601<nsecs>(tp);
    EXPECT_EQ(t.expected.str, actual);
  }
}
