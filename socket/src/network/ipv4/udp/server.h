#ifndef INCLUDE_NETWORK_IPV4_UDP_SERVER_H
#define INCLUDE_NETWORK_IPV4_UDP_SERVER_H
// Inherit
#include "network/ipv4/abc/socket_server.h"

namespace nw {
namespace ipv4 {
namespace udp {

class Server : public nw::ipv4::abc::SocketServer
{
public:
  Server(const std::string&, struct addrinfo);
  ~Server();

public:
  int Establish() override;
  bool Start() override;
};
} // namespace udp
} // namespace ipv4
} // namespace nw

#endif // INCLUDE_NETWORK_IPV4_UDP_SERVER_H
