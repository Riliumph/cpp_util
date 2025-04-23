#ifndef INCLUDE_NETWORK_IPV4_UDP_SERVER_H
#define INCLUDE_NETWORK_IPV4_UDP_SERVER_H
// Inherit
#include "network/ipv4/abc/socket_server.h"
// STL
#include <memory>
// Standard
#include <netdb.h>
// System
#include <sys/socket.h>
#include <sys/types.h>

namespace nw {
namespace ipv4 {
namespace udp {

class Server : public nw::ipv4::abc::SocketServer
{
public:
  Server(u_short, struct addrinfo);
  ~Server();

public:
  int Establish() override;
  bool Start() override;

private:
  bool CloseEvent(int);

protected: // Server Config
  struct timeval timeout_;
};

} // namespace udp
} // namespace ipv4
} // namespace nw

#endif // INCLUDE_NETWORK_IPV4_UDP_SERVER_H
