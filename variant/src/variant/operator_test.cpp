#include <string>
#include <thread>
// 3rd
#include <gtest/gtest.h>
// original
#include "operator.hpp"

TEST(Equal, variant_operator)
{
  int i = 1;
  EXPECT_EQ(i, 1);
}
