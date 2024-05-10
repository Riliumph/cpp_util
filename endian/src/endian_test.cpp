#include "endian.hpp"
// 3rd
#include <gtest/gtest.h>

TEST(little_to_big, byte4)
{
  typedef union
  {
    uint32_t b4;
    uint8_t b1[4];
  } bytes;

  struct test
  {
    bytes data;
    uint8_t expected[4];
  };

  struct test tt[] = { { 0x12345678, { 0x12, 0x34, 0x56, 0x78 } } };
  for (const auto& t : tt) {
    bytes actual;
    actual.b4 = ReverseEndian(t.data.b4);
    for (size_t i = 0; i < sizeof(bytes); ++i) {
      EXPECT_EQ(t.expected[i], actual.b1[i]);
    }
  }
}

TEST(little_to_big, byte8)
{
  typedef union
  {
    uint64_t b4;
    uint8_t b1[8];
  } bytes;

  struct test
  {
    bytes data;
    uint8_t expected[8];
  };

  struct test tt[] = {
    { 0x0000000012345678, { 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x56, 0x78 } },
    { 0x12345678, { 0x12, 0x34, 0x56, 0x78, 0x00, 0x00, 0x00, 0x00 } }
  };
  for (const auto& t : tt) {
    bytes actual;
    actual.b4 = ReverseEndian(t.data.b4);
    for (size_t i = 0; i < sizeof(bytes); ++i) {
      EXPECT_EQ(t.expected[i], actual.b1[i]);
    }
  }
}
