// STL
#include <cstring>
#include <string>
// system
#include <arpa/inet.h>
// 3rd
#include <gtest/gtest.h>
// original
#include "ipv4.hpp"

TEST(server, convert_ip)
{
  struct test_data
  {
    std::string name;
    struct addrinfo hint;
    std::string result;
  };

  std::vector<sockaddr_in> ipv4s = {
    // バージョンによってアライメントが変わっている感じがある
    { AF_INET, htons(8080), { inet_addr("127.0.0.1") }, { 0 } }
  };

  std::vector<test_data> tests = { { "ipv4",
                                     { AI_PASSIVE,
                                       AF_INET,
                                       SOCK_STREAM,
                                       IPPROTO_TCP,
                                       (socklen_t)sizeof(sockaddr_in),
                                       (struct sockaddr*)(&ipv4s[0]),
                                       nullptr,
                                       nullptr },
                                     "192.168.0.1" } };
  // test開始
  Server srv;
  for (const auto& test : tests) {
    auto err = srv.Identify(test.hint);
    if (err < 0) {
      FAIL() << "Identify: " << strerror(errno) << std::endl;
    }
    auto ip = srv.IP();
    EXPECT_EQ(test.result, ip);
  }
}
