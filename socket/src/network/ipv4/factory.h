#ifndef INCLUDE_NETWORK_IPV4_FACTORY_H
#define INCLUDE_NETWORK_IPV4_FACTORY_H
// STL
#include <memory>
// Standard
#include <netdb.h>
// original
#include "network/ipv4/tcp.h"
#include "network/ipv4/udp.h"

namespace nw {
namespace ipv4 {
std::unique_ptr<abc::SocketServer> MakeServer(u_short, struct addrinfo);
} // namespace ipv4
} // namespace nw
#endif // INCLUDE_NETWORK_IPV4_FACTORY_H
