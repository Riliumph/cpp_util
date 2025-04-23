#include "factory.h"

namespace nw {
namespace ipv4 {
//
std::unique_ptr<abc::SocketServer>
MakeServer(std::shared_ptr<event::IF::EventHandler> e,
           u_short p,
           struct addrinfo hint_)
{
  using TcpServer = nw::ipv4::tcp::Server;
  using UdpServer = nw::ipv4::udp::Server;
  // ipv4名前空間なのでipv4の確認は行わない
  // bool is_ipv4 = (hint_->ai_addr == AF_INET);
  bool is_tcp = (hint_.ai_socktype == SOCK_STREAM);
  bool is_udp = (hint_.ai_family == SOCK_DGRAM);
  if (is_tcp) {
    return std::make_unique<TcpServer>(e, p, hint_);
  } else if (is_udp) {
    return std::make_unique<UdpServer>(e, p, hint_);
  }
  static_assert("not supported server type");
  return nullptr;
}
} // namespace ipv4
} // namespace nw
