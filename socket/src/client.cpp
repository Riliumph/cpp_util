// STL
#include <iostream>
#include <string>
#include <vector>
// system
#include <unistd.h>
// original
#include "server/ipv4.hpp"

ssize_t
getline(int, std::string&);

int
main()
{
  struct addrinfo hint;
  hint.ai_family = AF_INET;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_flags = AI_PASSIVE;
  Server srv;
  srv.Identify(hint);
  if (srv.Socket() < 0) {
    return -1;
  }
  if (srv.Bind() < 0) {
    return -1;
  }
  if (srv.Listen() < 0) {
    return -1;
  }
  if (srv.Accept() < 0) {
    return -1;
  }
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
