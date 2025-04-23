#ifndef INCLUDE_NETWORK_IPV4_TCP_SERVER_H
#define INCLUDE_NETWORK_IPV4_TCP_SERVER_H
// Inherit
#include "network/ipv4/abc/socket_server.h"
// STL
#include <array>
#include <functional>
#include <memory>
#include <string>
// Standard
#include <netdb.h>
// System
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace nw::ipv4::tcp {
/// @brief IPv4でTCP通信を行うサーバー
class Server : public nw::ipv4::abc::SocketServer
{
  static const int QUEUE_SIZE = SOMAXCONN;
  static constexpr int CONNECTION_MAX = 20;
  static constexpr int DISABLE_FD = -1;

public: // constructor
  Server(u_short, struct addrinfo);
  ~Server();

public: // accessor
  void Timeout(struct timeval);
  void Timeout(time_t, suseconds_t);
  void Event(event::IF::EventHandler::callback) override;
  void EventHandler(std::shared_ptr<event::IF::EventHandler>) override;

public:
  int Establish() override;
  bool Start() override;

private:
  int AttachAddress();
  int Listen() const;
  int Accept() const;
  int CurrentConnection();
  int ControlMaxConnection(int);
  bool AcceptEvent(int);
  bool CloseEvent(int);

private: // File Descriptor
  std::array<int, CONNECTION_MAX> client_fds_;

private: // Event Handler
  std::shared_ptr<event::IF::EventHandler> event_handler_;
  event::IF::EventHandler::callback event_;

protected: // Server Config
  struct timeval timeout_;
};
}
#endif // INCLUDE_NETWORK_IPV4_TCP_SERVER_H
