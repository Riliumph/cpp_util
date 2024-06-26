#ifndef INCLUDE_TCP_SERVER_IPV4_H
#define INCLUDE_TCP_SERVER_IPV4_H
// STL
#include <functional>
#include <memory>
#include <string>
#include <vector>
// Standard
#include <netdb.h>
// System
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

/// @brief IPv4でTCP通信を行うサーバー
class Server
{
  static const int QUEUE_SIZE = SOMAXCONN;

public: // constructor
  Server();
  ~Server();

public: // accessor
  void Hint(struct addrinfo&);
  struct addrinfo Hint();
  std::string IP() const;
  int Port() const;
  void Port(u_short);
  void Timeout(struct timeval);
  void Timeout(time_t, suseconds_t);
  struct timeval Timeout() const;

public:
  int Identify(std::string);
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
  struct addrinfo* hint;

protected: // Server Config
  char host_name[NI_MAXHOST];
  char serv_name[NI_MAXSERV];
  struct timeval timeout;
};
#endif // INCLUDE_TCP_SERVER_H
