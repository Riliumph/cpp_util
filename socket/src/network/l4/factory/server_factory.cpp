#include "server_factory.h"
// STL
#include <cassert>
#include <iostream>

namespace nw {
namespace l4 {
std::unique_ptr<ServerFactory::server_t>
ServerFactory::MakeServer(const std::string& ip,
                          const std::string& port,
                          addr_t hint)
{
  std::cout << "create server..." << std::endl;
  // ipv4名前空間なのでipv4の確認は行わない
  // bool is_ipv4 = (hint_->ai_addr == AF_INET);
  bool is_tcp = (hint.ai_socktype == SOCK_STREAM);
  bool is_udp = (hint.ai_family == SOCK_DGRAM);
  if (is_tcp) {
    return std::make_unique<tcp_server_t>(ip, port, hint);
  }
  if (is_udp) {
    return std::make_unique<udp_server_t>(ip, port, hint);
  }
  assert(false && "not supported server type");
  return nullptr;
}
} // namespace l4
} // namespace nw
