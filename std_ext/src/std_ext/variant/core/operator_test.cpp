// STL
#include <string>
// 3rd
#include <gtest/gtest.h>
// std_ext
#include "operator.hpp"

using namespace std::literals::string_literals;

TEST(variant, operator_equal_primitive)
{
  std::variant<int> v = 1;
  auto result = v == 1;
  EXPECT_TRUE(result);
  result = 1 == v;
  EXPECT_TRUE(result);
}

TEST(variant, operator_equal_composite)
{
  std::variant<std::string> v = "foo";
  auto result2 = v == "foo"s;
  EXPECT_TRUE(result2);
}

TEST(variant, operator_plus_primitive)
{
  std::variant<int> v = 1;
  auto actual = std::get<int>(v) + 1;
  auto result = v + 1;
  EXPECT_EQ(actual, std::get<int>(result));
  result = 1 + v;
  EXPECT_EQ(actual, std::get<int>(result));
}

TEST(variant, operator_plus_composite)
{
  std::variant<int, std::string> v = "foo"s;
  auto actual = std::get<std::string>(v) + "bar"s;
  auto result = v + "bar"s;
  EXPECT_EQ(actual, std::get<std::string>(result));
  actual = "bar"s + std::get<std::string>(v);
  result = "bar"s + v;
  EXPECT_EQ(actual, std::get<std::string>(result));
}

TEST(variant, operator_minus_primitive)
{
  std::variant<int, std::string> v = 1;
  auto result = v - 1;
  EXPECT_EQ(0, std::get<int>(result));
  result = 1 - v;
  EXPECT_EQ(0, std::get<int>(result));
}

TEST(variant, operator_multiple_primitive)
{
  std::variant<int, std::string> v = 1;
  auto result = v * 2;
  EXPECT_EQ(2, std::get<int>(result));
  result = 2 * v;
  EXPECT_EQ(2, std::get<int>(result));
}

TEST(variant, operator_division_primitive)
{
  std::variant<int, std::string> v = 1;
  auto result = v / 2;
  EXPECT_EQ(0, std::get<int>(result));
  result = 2 * v;
  EXPECT_EQ(2, std::get<int>(result));
}
