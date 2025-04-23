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

public: // accessor
  void Event(event::IF::EventHandler::callback) override;
  void EventHandler(std::shared_ptr<event::IF::EventHandler>) override;

public:
  int Establish() override;
  bool Start() override;

private:
  int AttachAddress();

  bool CloseEvent(int);

private: // Event Handler
  std::shared_ptr<event::IF::EventHandler> event_handler_;
  event::IF::EventHandler::callback event_;

protected: // Server Config
  struct timeval timeout_;
};

} // namespace udp
} // namespace ipv4
} // namespace nw

#endif // INCLUDE_NETWORK_IPV4_UDP_SERVER_H
