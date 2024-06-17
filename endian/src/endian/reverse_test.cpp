#include "endian.hpp"
// STL
#include <string>
// 3rd
#include <gtest/gtest.h>

TEST(little_to_big, byte4)
{
  typedef union
  {
    uint32_t b4;
    uint8_t b1[sizeof(uint32_t)];
  } byte_data;
  struct TestData
  {
    byte_data data;
  };
  struct TestResult
  {
    uint8_t b1[sizeof(uint32_t)];
  };
  struct TestSet
  {
    std::string name;
    TestData arg;
    TestResult expected;
  };
  struct TestSet tt[] = {
    { "4byteデータの変換", 0x12345678, { 0x12, 0x34, 0x56, 0x78 } },
  };

  for (const auto& t : tt) {
    byte_data actual;
    actual.b4 = ReverseEndian(t.arg.data.b4);
    for (size_t i = 0; i < sizeof(byte_data); ++i) {
      EXPECT_EQ(t.expected.b1[i], actual.b1[i]);
    }
  }
}

TEST(little_to_big, byte8)
{
  typedef union
  {
    uint64_t b4;
    uint8_t b1[sizeof(uint64_t)];
  } byte_data;

  struct TestData
  {
    byte_data data;
  };

  struct TestResult
  {
    uint8_t b1[sizeof(uint64_t)];
  };

  struct TestSet
  {
    std::string name;
    TestData arg;
    TestResult expected;
  };
  struct TestSet tt[] = {
    { "8byte0埋めデータの変換",
      { 0x0000000012345678 },
      { 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x56, 0x78 } },
    { "8byteデータの変換",
      { 0x12345678 },
      { 0x12, 0x34, 0x56, 0x78, 0x00, 0x00, 0x00, 0x00 } },
  };

  for (const auto& t : tt) {
    byte_data actual;
    actual.b4 = ReverseEndian(t.arg.data.b4);
    for (size_t i = 0; i < sizeof(byte_data); ++i) {
      EXPECT_EQ(t.expected.b1[i], actual.b1[i]);
    }
  }
}
