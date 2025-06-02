#include "singleton.hpp"
// 3rd
#include <gtest/gtest.h>

TEST(singleton, rewrite)
{
  struct TestData
  {
    int data;
  };
  struct TestResult
  {
    int data;
  };
  struct TestSet
  {
    std::string name;
    std::function<void(decltype(TestData::data))> test;
    struct TestData arg;
    struct TestResult expected;
  };

  auto call_by_val = [](int d) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    auto only_one = singleton::Initializer<TestData>::Get();
#pragma GCC diagnostic pop
    only_one.data = d;
  };

  auto call_by_ref = [](int d) {
    // Initializer<TestData>::Get().data = dと同等
    auto& only_one = singleton::Initializer<TestData>::Get();
    only_one.data = d;
  };

  struct TestSet tt[] = {
    // シングルトンのデータが上書きされない試験、上書きされる試験の順番に定義すること
    { "値で受け取ってデータを上書きする", call_by_val, { 10 }, { 0 } },
    { "参照で受け取ってデータを上書きする", call_by_ref, { 10 }, { 10 } },
  };
  for (const auto& t : tt) {
    t.test(t.arg.data);
    auto new_v = singleton::Initializer<TestData>::Get().data;
    // テスト関数の実行後に新規で取得した値は期待する値であること
    EXPECT_EQ(t.expected.data, new_v);
  }
}
