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

  std::vector<test_data> tests = { { "NULLを指定しているため0.0.0.0である",
                                     { AI_PASSIVE,
                                       AF_INET,
                                       SOCK_STREAM,
                                       IPPROTO_TCP,
                                       (socklen_t)sizeof(sockaddr_in),
                                       (struct sockaddr*)(&ipv4s[0]),
                                       nullptr,
                                       nullptr },
                                     "0.0.0.0" } };
  // test開始
  Server srv;
  srv.Port(8080);
  for (auto& test : tests) {
    auto err = srv.Identify(test.hint);
    if (err < 0) {
      FAIL() << "Identify: " << strerror(errno) << std::endl;
    }
    auto ip = srv.IP();
    EXPECT_EQ(test.result, ip);
  }
}

TEST(server, get_port_number)
{
  struct test_data
  {
    std::string name;
    struct addrinfo hint;
    int port_no;
  };

  std::vector<sockaddr_in> ipv4s = {
    // バージョンによってアライメントが変わっている感じがある
    { AF_INET, htons(8080), { inet_addr("127.0.0.1") }, { 0 } }
  };

  std::vector<test_data> tests = { { "正常系：ポート番号が正しいか",
                                     { AI_PASSIVE,
                                       AF_INET,
                                       SOCK_STREAM,
                                       IPPROTO_TCP,
                                       (socklen_t)sizeof(sockaddr_in),
                                       (struct sockaddr*)(&ipv4s[0]),
                                       nullptr,
                                       nullptr },
                                     8080 } };
  // test開始
  Server srv;
  srv.Port(8080);
  for (auto& test : tests) {
    auto err = srv.Identify(test.hint);
    if (err < 0) {
      FAIL() << "Identify: " << strerror(errno) << std::endl;
    }
    auto port_no = srv.Port();
    EXPECT_EQ(test.port_no, port_no);
  }
}
