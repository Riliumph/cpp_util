#include "socket_server.h"

namespace nw {
namespace ipv4 {
namespace abc {
SocketServer::SocketServer(u_short port, struct addrinfo hint)
  : server_fd_{ 0 }
  , port_{ port }
  , inet0_{ new struct addrinfo }
  , hint_{ new struct addrinfo }
{
  (void)hint;
}

} // namespace abc
} // namespace ipv4
} // namespace nw
