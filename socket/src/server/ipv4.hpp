#ifndef TCP_SERVER_IPV4_HPP
#define TCP_SERVER_IPV4_HPP
// STL
#include <functional>
#include <memory>
#include <vector>
// System
#include <sys/select.h>
#include <sys/time.h>

namespace tcp {
namespace ipv4 {
/// @brief IPv4でTCP通信を行うサーバー
class Server
{
public: // constructor
  Server();
  ~Server();

public: // accessor
  std::string IP() const;
  int Port() const;
  void Port(u_short);
  void Timeout(struct timeval);
  void Timeout(time_t, suseconds_t);
  struct timeval Timeout() const;

public:
  int Identify(struct addrinfo, u_short);
  bool Establish();
  bool ReuseAddress();
  bool Listen();
  int Accept();

  bool LoopBySelect(std::function<bool(int)>);
#ifdef EPOLL
  bool LoopByEPoll(std::function<bool(int)>);
#endif // EPOLL

private: // File Descriptor
  fd_set fds;
  int fd; // サーバー接続を待ち受けているソケットFD

protected: // IP config
  std::shared_ptr<struct addrinfo> inet0;
  std::string ip;
  u_short port_no;

protected: // Server Config
  char host_name[NI_MAXHOST];
  char serv_name[NI_MAXSERV];
  struct timeval timeout;
};
} // namespace ipv4
} // namespace tcp
#endif // TCP_SERVER_HPP
