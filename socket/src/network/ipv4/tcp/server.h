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
  int Listen() const;
  int Accept() const;
  int CurrentConnection();
  int ControlMaxConnection(int);
  bool AcceptEvent(int);
  bool CloseEvent(int);

private:          // File Descriptor
  int server_fd_; // サーバー接続を待ち受けているソケットFD
  std::array<int, CONNECTION_MAX> client_fds_;

private: // Event Handler
  std::shared_ptr<event::IF::EventHandler> event_handler_;
  event::IF::EventHandler::callback event_;

protected: // IP config
  u_short port_;
  struct addrinfo* inet0_; // Linux変数はC++で定義しない
  struct addrinfo* hint_;

protected: // Server Config
  char host_name_[NI_MAXHOST];
  char serv_name_[NI_MAXSERV];
  struct timeval timeout_;
};
}
#endif // INCLUDE_NETWORK_IPV4_TCP_SERVER_H
