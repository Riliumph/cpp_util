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
  Server(std::shared_ptr<event::IF::EventHandler>, u_short, struct addrinfo);
  ~Server();

public: // accessor
  void Timeout(struct timeval);
  void Timeout(time_t, suseconds_t);
  void Event(event::IF::EventHandler::callback) override;

public:
  int Establish() override;
  bool Start() override;

private:
  void Hint(const struct addrinfo&);
  void SafeClose();

private:
  int Identify(std::string = "");
  int CreateSocket();
  int AttachAddress();

  bool CloseEvent(int);

private:         // File Descriptor
  int server_fd; // サーバー接続を待ち受けているソケットFD

private: // Event Handler
  std::shared_ptr<event::IF::EventHandler> event_handler;
  event::IF::EventHandler::callback event;

protected: // IP config
  u_short port_;
  struct addrinfo* inet0; // Linux変数はC++で定義しない
  struct addrinfo* hint;

protected: // Server Config
  char host_name[NI_MAXHOST];
  char serv_name[NI_MAXSERV];
  struct timeval timeout;
};

} // namespace udp
} // namespace ipv4
} // namespace nw

#endif // INCLUDE_NETWORK_IPV4_UDP_SERVER_H
