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
std::unique_ptr<abc::SocketServer>
MakeServer(const std::string&, const std::string&, struct addrinfo);
} // namespace l4
} // namespace nw
#endif // INCLUDE_NETWORK_IPV4_FACTORY_H
