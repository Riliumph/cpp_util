#ifndef TCP_SERVER_IPV4_HPP
#define TCP_SERVER_IPV4_HPP
// STL
#include <functional>
#include <memory>
#include <vector>
// System
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>

/// @brief IPv4でTCP通信を行うサーバー
class Server
{
  static const int QUEUE_SIZE = SOMAXCONN;

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
  int Identify(struct addrinfo&, std::string = "");
  int Socket();
  int Bind();
  int Listen();
  int Accept();

  bool LoopBySelect(std::function<bool(int)>);
#ifdef EPOLL
  bool LoopByEPoll(std::function<bool(int)>);
#endif // EPOLL

private:
  int ReuseAddress();
  void CloseClient(int);
  void SafeClose();

private: // File Descriptor
  fd_set fds;
  int server_fd; // サーバー接続を待ち受けているソケットFD
  std::vector<int> client_fds;

protected: // IP config
  std::string ip;
  u_short port_;
  struct addrinfo* inet0; // Linux変数はC++で定義しない

protected: // Server Config
  char host_name[NI_MAXHOST];
  char serv_name[NI_MAXSERV];
  struct timeval timeout;
};
#endif // TCP_SERVER_HPP
