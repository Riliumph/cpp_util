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

typedef union
{
  uint32_t byte4;
  uint16_t byte2[2];
  uint8_t byte1[4];
} bytes;

std::ostream&
operator<<(std::ostream& os, bytes lv)
{
  std::printf("DEC: %d\n", lv.byte4);
  std::printf("byte(4): 0x%08X\n", lv.byte4);
  std::printf("byte(2): 0x%08X, 0x%08X\n", lv.byte2[0], lv.byte2[1]);
  std::printf("byte(1): 0x%08X, 0x%08X, 0x%08X, 0x%08X\n",
              lv.byte1[0],
              lv.byte1[1],
              lv.byte1[2],
              lv.byte1[3]);
  return os;
}

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

    bytes raw1_b;
    raw1_b.byte4 = raw1;

    auto data1 = endian::Reverse(raw1);
    auto data2 = endian::Reverse(raw2);
    auto data3 = endian::Reverse(raw3);
    auto data4 = endian::Reverse(raw4);

    bytes raw1_b_;
    raw1_b_.byte4 = data1;

    std::printf("1:%+10d(0x%08x)\n->%+10d(0x%08x)\n", raw1, raw1, data1, data1);
    std::printf("2:%+10d(0x%08x)\n->%+10d(0x%08x)\n", raw2, raw2, data2, data2);
    std::printf("3:%+10d(0x%08x)\n->%+10d(0x%08x)\n", raw3, raw3, data3, data3);
    std::printf("4:%+10d(0x%08x)\n->%+10d(0x%08x)\n", raw4, raw4, data4, data4);

    std::cout << "raw1_b\n" << raw1_b << std::endl;
    std::cout << "raw1_b_\n" << raw1_b_ << std::endl;

    return true;
  });
}
