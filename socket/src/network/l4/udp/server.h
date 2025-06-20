#ifndef INCLUDE_NETWORK_L4_UDP_SERVER_H
#define INCLUDE_NETWORK_L4_UDP_SERVER_H
// Inherit
#include "network/l4/abc/socket_server.h"

namespace nw {
namespace l4 {
namespace udp {

class Server : public nw::l4::abc::SocketServer
{
public:
  Server(const std::string&, const std::string&, struct addrinfo);
  ~Server();

public:
  int Establish() override;
  bool Start() override;
};
} // namespace udp
} // namespace l4
} // namespace nw

#endif // INCLUDE_NETWORK_L4_UDP_SERVER_H
