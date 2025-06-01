#ifndef INCLUDE_NETWORK_L4_TCP_SERVER_H
#define INCLUDE_NETWORK_L4_TCP_SERVER_H
// Inherit
#include "network/l4/abc/socket_server.h"
// STL
#include <array>

namespace nw {
namespace l4 {
namespace tcp {
/// @brief IPv4でTCP通信を行うサーバー
class Server : public nw::l4::abc::SocketServer
{
  static const int QUEUE_SIZE = SOMAXCONN;
  static constexpr int CONNECTION_MAX = 20;

public: // constructor
  Server(const std::string&, const std::string&, struct addrinfo);
  ~Server();

public:
  int Establish() override;
  bool Start() override;

private:
  int Listen() const;
  int Accept() const;
  int CurrentConnection();
  int ControlMaxConnection(int);
  bool AcceptEvent(int);
  bool CloseEvent(int);

private: // File Descriptor
  std::array<int, CONNECTION_MAX> client_fds_;
};
} // namespace tcp
} // namespace l4
} // namespace nw
#endif // INCLUDE_NETWORK_L4_TCP_SERVER_H
