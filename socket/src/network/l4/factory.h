#ifndef INCLUDE_NETWORK_IPV4_FACTORY_H
#define INCLUDE_NETWORK_IPV4_FACTORY_H
// STL
#include <memory>
// Standard
#include <netdb.h>
// original
#include "network/l4/tcp.h"
#include "network/l4/udp.h"

namespace nw {
namespace l4 {
class ServerFactory
{
public:
  using event_handler_t = event::abc::EventHandler;
  using server_t = abc::SocketServer;
  using tcp_server_t = nw::l4::tcp::Server;
  using udp_server_t = nw::l4::udp::Server;
  using addr_t = struct addrinfo;

public:
  ServerFactory() = default;
  ~ServerFactory() = default;

  std::unique_ptr<server_t> MakeServer(const std::string&,
                                       const std::string&,
                                       addr_t);
};
} // namespace l4
} // namespace nw
#endif // INCLUDE_NETWORK_IPV4_FACTORY_H
