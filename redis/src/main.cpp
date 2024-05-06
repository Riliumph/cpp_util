// STL
#include <iostream>
// 3rd
#include <sw/redis++/redis++.h>
// original
#include "redis_api.hpp"

int
main()
{
  // redis への接続
  auto redis = sw::redis::Redis("tcp://127.0.0.1:6379");

  redis::health_check(redis);
  redis::set_data(redis);
  redis::set_data_with_expiration(redis);
  redis::input_list_data(redis);
  redis::input_capped_list_data(redis);
  redis::operate_queue(redis);
  redis::operate_hash(redis);
  redis::operate_json1(redis);
  std::cout << "終了" << std::endl;
  return 0;
};
