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
  auto eh = std::make_shared<event::EpollHandler>();
  eh->Timeout(std::nullopt);
  std::cout << "create server" << std::endl;
  nw::ipv4::tcp::Server srv(eh, port_no, hint);
  srv.Event([](int fd) {
    char buffer[1024];
    auto recv_size = read(fd, buffer, sizeof(buffer));
    std::cout << std::string(buffer) << std::endl;
      // std::array<uint8_t, 12> buffer;
      // std::cout << "recv" << std::endl;
      // auto recv_size = read(fd, buffer.data(), buffer.size());
      // if (recv_size < 0) {
      //   return;
      // } else if (recv_size == 0) {
      //   std::cout << "receive EOS." << std::endl;
      //   return;
      // } else if (0 < recv_size && recv_size < (ssize_t)buffer.size()) {
      //   std::cout << "incomplete receive" << std::endl;
      //   return;
      // }

      // int32_t raw1, raw2;
      // int16_t raw3, raw4;
      // int start_point = 0;
      // std::memcpy(&raw1, buffer.data() + start_point, sizeof(raw1));
      // start_point += sizeof(raw1);
      // std::memcpy(&raw2, buffer.data() + start_point, sizeof(raw2));
      // start_point += sizeof(raw2);
      // std::memcpy(&raw3, buffer.data() + start_point, sizeof(raw3));
      // start_point += sizeof(raw3);
      // std::memcpy(&raw4, buffer.data() + start_point, sizeof(raw4));

      // endian::bytes raw1_b;
      // raw1_b.byte4 = raw1;

      // auto data1 = endian::Reverse(raw1);
      // auto data2 = endian::Reverse(raw2);
      // auto data3 = endian::Reverse(raw3);
      // auto data4 = endian::Reverse(raw4);

      // endian::bytes raw1_b_;
      // raw1_b_.byte4 = data1;

      // std::printf("1:%+10d(0x%08x)\n->%+10d(0x%08x)\n", raw1, raw1, data1,
      // data1); std::printf("2:%+10d(0x%08x)\n->%+10d(0x%08x)\n", raw2, raw2,
      // data2, data2); std::printf("3:%+10d(0x%08x)\n->%+10d(0x%08x)\n", raw3,
      // raw3, data3, data3); std::printf("4:%+10d(0x%08x)\n->%+10d(0x%08x)\n",
      // raw4, raw4, data4, data4);

      // std::cout << "raw1_b\n" << raw1_b << std::endl;
      // std::cout << "raw1_b_\n" << raw1_b_ << std::endl;

      return;
  });
  std::cout << "establish server" << std::endl;
  if (srv.Establish() < 0) {
    std::cerr << "failed to establish server" << std::endl;
    return -1;
  }

  std::cout << "start server" << std::endl;
  srv.Start();
}
