#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <sw/redis++/redis++.h>
#include <vector>

int
main()
{
  // redis への接続
  auto redis = sw::redis::Redis("tcp://127.0.0.1:6379");

  // http://mogile.web.fc2.com/redis/commands/ping.html
  std::cout << "redis に ping を打つ" << std::endl;
  std::cout << redis.ping() << std::endl;

  // string 型
  std::cout << "string 型" << std::endl;
  redis.set("key", "val");
  auto val = redis.get("key");

  if (val) {
    std::cout << *val << std::endl;
  }

  auto del_result = redis.del("key");

  // list 型
  std::cout << "list 型" << std::endl;
  std::vector<std::string> hoges = { "nanigashi", "soregashi", "something" };
  redis.rpush("hoges", hoges.begin(), hoges.end());

  hoges.clear();
  redis.lrange("hoges", 0, -1, std::back_inserter(hoges));

  if (!hoges.empty())
    for (std::string str : hoges)
      std::cout << str << " ";
  std::cout << std::endl;

  del_result = redis.del("hoges");
  if (del_result) {
    std::cout << "(interger) " << del_result << std::endl;
  }

  hoges.clear();

  // Capped lists (上限付リスト)
  std::cout << "上限付リスト" << std::endl;
  hoges = { "nanigashi", "soregashi", "something", "hoge", "fuga" };

  auto rpush_result = redis.rpush("hoges", hoges.begin(), hoges.end());
  std::cout << "(interger) " << rpush_result << std::endl;

  hoges.clear();
  redis.lrange("hoges", 0, -1, std::back_inserter(hoges));

  if (!hoges.empty())
    for (std::string str : hoges)
      std::cout << str << " ";
  std::cout << std::endl;

  std::cout << "指定範囲外の要素を削除" << std::endl;
  redis.ltrim("hoges", 0, 2);

  hoges.clear();
  redis.lrange("hoges", 0, -1, std::back_inserter(hoges));

  if (!hoges.empty())
    for (std::string str : hoges)
      std::cout << str << " ";
  std::cout << std::endl;

  del_result = redis.del("hoges");

  // キューの操作
  std::cout << "キューの操作" << std::endl;
  hoges = { "nanigashi", "soregashi", "something" };

  redis.lpush("hoges", hoges.begin(), hoges.end());
  auto get_hoges = redis.rpop("hoges");
  std::cout << *get_hoges << std::endl;

  get_hoges = redis.rpop("hoges");
  std::cout << *get_hoges << std::endl;
  redis.del("hoges");

  std::cout << "5 秒後にタイムアウト" << std::endl;
  redis.brpop("tasks", 5);
  std::cout << "終了" << std::endl;
}
