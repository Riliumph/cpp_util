#include <iomanip>
#include <iostream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <sw/redis++/redis++.h>
#include <vector>

template<typename T>
std::ostream&
operator<<(std::ostream& os, const std::vector<T>& vs)
{
  if (vs.empty()) {
    os << "[]";
    return os;
  }
  os << "[" << vs.front();
  for (auto it = ++vs.begin(); it != vs.end(); ++it) {
    os << ", " << *it;
  }
  os << "]";
  return os;
}

void
ping_to_redis(sw::redis::Redis& client)
{
  std::cout << "#疎通確認" << std::endl;
  try {
    std::cout << client.ping() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void
input_data(sw::redis::Redis& client)
{
  std::cout << "# 通常の操作" << std::endl;
  std::pair<std::string, double> p = { "double_value", 1.2345678912345678 };
  try {
    {
      // set
      std::cout << "set data: " << p.second << std::endl;
      std::stringstream ss;
      ss << std::setprecision(20) << p.second;
      client.set(p.first, ss.str());
    }
    {
      // get
      std::cout << "get data: ";
      auto ret = client.get(p.first); // std::optional型で返ってくる
      if (!ret) {
        std::cerr << "(none)" << std::endl;
      } else {
        std::cout << *ret << std::endl;
      }
    }
    // close
    std::cout << client.del(p.first) << "件削除" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void
input_data_with_expiration(sw::redis::Redis& client)
{
  std::cout << "# 期限付きデータの操作" << std::endl;
  std::pair<std::string, std::string> p = { "expired_key", "expired_val" };
  auto sleep_time = std::chrono::seconds(3);
  try {
    // set
    std::cout << "set data: " << p.second << std::endl;
    client.set(p.first, p.second, sleep_time); // 3sec後に自動削除
    std::cout << "waiting... " << std::endl;
    std::this_thread::sleep_for(sleep_time);
    // get
    auto ret = client.get(p.first); // std::optional型で返ってくる
    if (!ret) {
      std::cout << "自動削除されて取得できない" << std::endl;
    } else {
      std::cerr << "見つかると消えてない" << std::endl;
    }
    // auto del_result = client.del(p.first); 自動削除される
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void
input_list_data(sw::redis::Redis& client)
{
  std::cout << "# list型の操作" << std::endl;
  std::pair<std::string, std::vector<std::string>> p = {
    "texts", { "foo", "bar", "hoge", "fuga" }
  };
  try {
    // set
    std::cout << "set data: " << p.second << std::endl;
    client.rpush(p.first, p.second.begin(), p.second.end());
    // get
    std::vector<std::string> ret_texts;
    client.lrange(p.first, 0, -1, std::back_inserter(ret_texts));
    std::cout << "get data: " << ret_texts << std::endl;
    // close
    std::cout << client.del(p.first) << " 件削除" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void
input_capped_list_data(sw::redis::Redis& client)
{
  std::cout << "# 上限付リスト" << std::endl;
  std::pair<std::string, std::vector<std::string>> p = {
    "texts", { "foo", "bar", "hoge", "fuga" }
  };
  try {
    // set data
    std::cout << "set data: " << p.second << std::endl;
    auto rpush_result = client.rpush(p.first, p.second.begin(), p.second.end());
    std::cout << rpush_result << "件入力" << std::endl;

    // get data
    std::vector<std::string> ret_texts;
    client.lrange(p.first, 0, -1, std::back_inserter(ret_texts));
    std::cout << "get data: " << ret_texts << std::endl;

    std::cout << "## 指定範囲外の要素を削除" << std::endl;
    client.ltrim(p.first, 0, 2);

    // get data
    ret_texts.clear();
    client.lrange(p.first, 0, -1, std::back_inserter(ret_texts));
    std::cout << "get data: " << ret_texts << std::endl;
    // close
    std::cout << client.del(p.first) << " 件削除" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void
operate_queue(sw::redis::Redis& client)
{
  std::cout << "# キューの操作" << std::endl;
  std::pair<std::string, std::vector<std::string>> p = {
    "texts", { "nanigashi", "soregashi", "something" }
  };
  try {
    // set data
    std::cout << "set data: " << p.second << std::endl;
    client.lpush(p.first, p.second.begin(), p.second.end());

    // get datas
    std::cout << "get data: ";
    for (size_t i = 0; i < p.second.size(); ++i) {
      auto get_texts = client.rpop(p.first);
      if (get_texts) {
        std::cout << *get_texts << ", ";
      } else {
        std::cout << "(none), ";
      }
    }
    std::cout << std::endl;

    // close
    std::cout << client.del(p.first) << " 件削除" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void
operate_blocking_pop(sw::redis::Redis& client)
{
  try {
    std::cout << "5 秒後にタイムアウト" << std::endl;
    client.brpop("tasks", 5);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

int
main()
{
  // redis への接続
  auto redis = sw::redis::Redis("tcp://127.0.0.1:6379");

  ping_to_redis(redis);
  input_data(redis);
  input_data_with_expiration(redis);
  input_list_data(redis);
  input_capped_list_data(redis);
  operate_queue(redis);
  operate_blocking_pop(redis);

  std::cout << "終了" << std::endl;
  return 0;
};
