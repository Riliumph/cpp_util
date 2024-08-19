#ifndef INCLUDE_NETWORK_IPV4_TCP_SERVER_H
#define INCLUDE_NETWORK_IPV4_TCP_SERVER_H
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
// Original
#include "network/interface/server.h"

namespace nw::ipv4::tcp {
/// @brief IPv4でTCP通信を行うサーバー
class Server : public nw::IF::Server
{
  static const int QUEUE_SIZE = SOMAXCONN;
  static constexpr int CONNECTION_MAX = 20;
  static constexpr int DISABLE_FD = -1;

public: // constructor
  Server();
  Server(const u_short, const struct addrinfo);
  ~Server();

public: // accessor
  void Timeout(struct timeval);
  void Timeout(time_t, suseconds_t);

public:
  int Establish() override;

  bool LoopBySelect(std::function<bool(int)>);
#ifdef EPOLL
  bool LoopByEPoll(std::function<bool(int)>);
#endif // EPOLL

private:
  void Hint(const struct addrinfo&);
  struct timeval* Timeout();
  void SafeClose();

private:
  int Identify(std::string = "");
  int CreateSocket();
  int AttachAddress();
  int Listen();
  int Accept();
  int CurrentConnection();
  int ControlMaxConnection(const int);

private: // File Descriptor
  fd_set fds;
  int server_fd; // サーバー接続を待ち受けているソケットFD
  std::array<int, CONNECTION_MAX> client_fds;

protected: // IP config
  std::string ip;
  u_short port_;
  struct addrinfo* inet0; // Linux変数はC++で定義しない
  struct addrinfo* hint;

protected: // Server Config
  char host_name[NI_MAXHOST];
  char serv_name[NI_MAXSERV];
  struct timeval timeout;
};
}
#endif // INCLUDE_NETWORK_IPV4_TCP_SERVER_H
