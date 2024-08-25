// STL
#include <cstdio>
#include <cstring>
#include <iostream>
// system
#include <unistd.h>
// 3rd
#include "endian.hpp"
// original
#include "network.h"

int
main()
{
  u_short port_no = 50000;
  struct addrinfo hint;
  hint.ai_family = AF_INET;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_flags = AI_PASSIVE;
  std::cout << "create server" << std::endl;
  nw::ipv4::tcp::Server srv(port_no, hint);
  std::cout << "establish server" << std::endl;
  if (srv.Establish() < 0) {
    std::cerr << "failed to establish server" << std::endl;
    return -1;
  }
  srv.LoopBySelect([](int fd) {
    std::array<uint8_t, 12> buffer;
    std::cout << "recv" << std::endl;
    auto recv_size = read(fd, buffer.data(), buffer.size());
    if (recv_size < 0) {
      return false;
    } else if (recv_size == 0) {
      std::cout << "receive EOS." << std::endl;
      return false;
    } else if (0 < recv_size && recv_size < (ssize_t)buffer.size()) {
      std::cout << "incomplete receive" << std::endl;
      return false;
    }

    int32_t raw1, raw2;
    int16_t raw3, raw4;
    int start_point = 0;
    std::memcpy(&raw1, buffer.data() + start_point, sizeof(raw1));
    start_point += sizeof(raw1);
    std::memcpy(&raw2, buffer.data() + start_point, sizeof(raw2));
    start_point += sizeof(raw2);
    std::memcpy(&raw3, buffer.data() + start_point, sizeof(raw3));
    start_point += sizeof(raw3);
    std::memcpy(&raw4, buffer.data() + start_point, sizeof(raw4));

    auto data1 = endian::Reverse(raw1);
    auto data2 = endian::Reverse(raw2);
    auto data3 = endian::Reverse(raw3);
    auto data4 = endian::Reverse(raw4);

    std::printf("1:%+10d(0x%08x)\n->%+10d(0x%08x)\n", raw1, raw1, data1, data1);
    std::printf("2:%+10d(0x%08x)\n->%+10d(0x%08x)\n", raw2, raw2, data2, data2);
    std::printf("3:%+10d(0x%08x)\n->%+10d(0x%08x)\n", raw3, raw3, data3, data3);
    std::printf("4:%+10d(0x%08x)\n->%+10d(0x%08x)\n", raw4, raw4, data4, data4);
    return true;
  });
}
