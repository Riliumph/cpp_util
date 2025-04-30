#include "factory.h"

namespace nw {
namespace l4 {
//
std::unique_ptr<abc::SocketServer>
MakeServer(const std::string& ip, const std::string& port, struct addrinfo hint)
{
  using TcpServer = nw::l4::tcp::Server;
  using UdpServer = nw::l4::udp::Server;
  // ipv4名前空間なのでipv4の確認は行わない
  // bool is_ipv4 = (hint_->ai_addr == AF_INET);
  bool is_tcp = (hint.ai_socktype == SOCK_STREAM);
  bool is_udp = (hint.ai_family == SOCK_DGRAM);
  if (is_tcp) {
    return std::make_unique<TcpServer>(ip, port, hint);
  } else if (is_udp) {
    return std::make_unique<UdpServer>(ip, port, hint);
  }
  static_assert("not supported server type");
  return nullptr;
}
} // namespace l4
} // namespace nw
