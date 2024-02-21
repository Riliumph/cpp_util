#include <iomanip>
#include <iostream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <sw/redis++/redis++.h>
#include <vector>

int
main()
{
  // redis への接続
  auto redis = sw::redis::Redis("tcp://127.0.0.1:6379");

  try {
    std::cout << "#疎通確認" << std::endl;
    std::cout << redis.ping() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  try {
    std::cout << "# 通常の操作" << std::endl;
    std::pair<std::string, double> p = { "double_value", 1.2345678912345678 };
    std::stringstream ss;
    ss << std::setprecision(20) << p.second;
    redis.set(p.first, ss.str());
    auto ret = redis.get(p.first); // std::optional型で返ってくる
    if (!ret) {
      std::cerr << "not found value" << std::endl;
    } else {
      std::cout << *ret << std::endl;
    }
    std::cout << redis.del(p.first) << "件削除" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  try {
    std::cout << "# 期限付きデータの操作" << std::endl;
    auto sleep_time = std::chrono::seconds(3);
    std::pair<std::string, std::string> p = { "expired_key", "expired_val" };
    redis.set(p.first, p.second, sleep_time); // 3sec後に自動削除
    std::this_thread::sleep_for(sleep_time);
    auto ret = redis.get(p.first); // std::optional型で返ってくる
    if (!ret) {
      std::cout << "自動削除されて取得できない" << std::endl;
    } else {
      std::cerr << "見つかると消えてない" << std::endl;
    }
    // auto del_result = redis.del(p.first); 自動削除される
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  // list 型
  try {
    std::cout << "# list型の操作" << std::endl;
    std::pair<std::string, std::vector<std::string>> p = {
      "texts", { "foo", "bar", "hoge", "fuga" }
    };
    // set
    redis.rpush(p.first, p.second.begin(), p.second.end());
    std::cout << "set data: ";
    for (const auto& t : p.second) {
      std::cout << t << " ";
    }
    std::cout << std::endl;

    // get
    std::vector<std::string> ret_texts;
    redis.lrange(p.first, 0, -1, std::back_inserter(ret_texts));
    std::cout << "get data: ";
    if (!ret_texts.empty()) {
      for (const auto& t : ret_texts) {
        std::cout << t << " ";
      }
      std::cout << std::endl;
    } else {
      std::cout << "(nothing)" << std::endl;
    }

    // close
    std::cout << redis.del(p.first) << " 件削除" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  // Capped lists (上限付リスト)
  try {
    std::cout << "# 上限付リスト" << std::endl;
    auto key = "texts";
    std::vector<std::string> texts = { "foo", "bar", "hoge", "fuga" };
    // set value
    std::cout << "set value: ";
    for (const auto& t : texts) {
      std::cout << t << " ";
    }
    std::cout << std::endl;

    auto rpush_result = redis.rpush(key, texts.begin(), texts.end());
    std::cout << rpush_result << "件入力" << std::endl;

    // get value
    std::vector<std::string> ret_texts;
    redis.lrange(key, 0, -1, std::back_inserter(ret_texts));

    std::cout << "get value: ";
    if (!ret_texts.empty()) {
      for (std::string str : ret_texts) {
        std::cout << str << " ";
      }
    }
    std::cout << std::endl;

    std::cout << "## 指定範囲外の要素を削除" << std::endl;
    ret_texts.clear();
    redis.ltrim(key, 0, 2);

    // get value
    redis.lrange(key, 0, -1, std::back_inserter(ret_texts));
    std::cout << "get value: ";
    if (!ret_texts.empty()) {
      for (const auto& r : ret_texts) {
        std::cout << r << " ";
      }
    }
    std::cout << std::endl;

    // close
    auto del_result = redis.del("texts");
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  // キューの操作
  try {
    std::cout << "# キューの操作" << std::endl;
    auto key = "texts";
    std::vector<std::string> texts = { "nanigashi", "soregashi", "something" };

    // set value
    std::cout << "set value: ";
    for (const auto& t : texts) {
      std::cout << t << " ";
    }
    std::cout << std::endl;
    redis.lpush(key, texts.begin(), texts.end());

    // get values
    for (size_t i = 0; i < texts.size(); ++i) {
      auto get_texts = redis.rpop(key);
      std::cout << i << "番目: " << *get_texts << std::endl;
    }

    // close
    redis.del("texts");
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  try {
    std::cout << "5 秒後にタイムアウト" << std::endl;
    redis.brpop("tasks", 5);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  std::cout << "終了" << std::endl;
  return 0;
};
