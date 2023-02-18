// STL
#include <iostream>
#include <string>
#include <vector>
// system
#include <unistd.h>
// original
#include "tcp/server/ipv4.hpp"

ssize_t
getline(int, std::string&);

int
main()
{
  tcp::ipv4::Server server("127.0.0.1", 12345, SOCK_STREAM);
#ifndef EPOLL
  if (!server.Establish()) {
    std::cerr << "cannot establish" << std::endl;
    return -1;
  }
  if (!server.ReuseAddress()) {
    std::cerr << "cannot apply option" << std::endl;
    return -1;
  }
  if (!server.Listen()) {
    std::cerr << "cannot listen" << std::endl;
    return -1;
  }
  std::cout << "Server Info" << std::endl;
  std::cout << "IP  : " << server.IP() << std::endl;
  std::cout << "Port: " << server.Port() << std::endl;

  server.LoopBySelect([](int fd) {
    static const int max_buf = 5;
    std::string buf;
    buf.resize(max_buf);
    auto read_size = getline(fd, buf);
    if (0 < read_size) {
      std::cout << buf << std::endl;
      return true;
    }
    return false;
  });
#else
  server.LoopByEPoll([](int fd) {
    static const int max_buf = 5;
    std::string buf;
    buf.resize(max_buf);
    auto read_size = getline(fd, buf);
    if (0 < read_size) {
      std::cout << buf << std::endl;
      return true;
    }
    return false;
  });
#endif
  return 0;
}

/// @brief 一行を読み込む関数
/// @param fd fd
/// @param buffer buffer
/// @return 読み込み総サイズ
ssize_t
getline(int fd, std::string& buffer)
{
  std::size_t whole_byte = 0;
  auto byte = read(fd, buffer.data(), buffer.length());
  whole_byte += byte;

  // auto constexpr read_size = 1;
  // char c;
  //  while (whole_byte < buffer.size()) {
  //  again:
  //    auto byte = read(fd, &c, read_size);
  //    whole_byte += byte;
  //    if (byte == 0) {
  //      if (whole_byte == 1) {
  //        return 0;
  //      }
  //      break;
  //    } else if (byte == read_size) {
  //      buffer += c;
  //      if (c == '\n') {
  //        break;
  //      }
  //    }
  //    if (errno == EINTR) {
  //      goto again;
  //    }
  //    return -1;
  //  }
  return whole_byte;
}
