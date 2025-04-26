#ifndef INCLUDE_NETWORK_IPV4_ABC_SOCKET_SERVER_H
#define INCLUDE_NETWORK_IPV4_ABC_SOCKET_SERVER_H
// super class
#include "network/interface/server.h"
// STL
#include <memory>
// Standard
#include <netdb.h>
// Original Lib
#include <event.hpp>

namespace nw {
namespace ipv4 {
using event_handler_if_t = event::interface::EventHandler;
using event_callback_t = event_handler_if_t::callback_t;
namespace abc {
/// @brief ソケットサーバーの抽象基底クラス
/// @details TCP/UDPサーバーが継承する基底クラスである。
/// TCP/UDPサーバーの共通処理が実装される。
class SocketServer : public nw::IF::Server
{
protected:
  using fd_t = int;
  /// @brief FDの無効値
  static constexpr fd_t DISABLE_FD = -1;

public:
  ~SocketServer();
  void EventHandler(std::shared_ptr<event_handler_if_t>);
  void Event(event_callback_t);
  virtual int Establish() = 0;
  virtual bool Start() = 0;

protected:
  SocketServer(const std::string&, const std::string&, struct addrinfo);
  int CreateSocket();
  int AttachAddress();

private:
  int Identify();
  void Hint(const struct addrinfo&);
  void SafeClose();

protected:
  fd_t server_fd_;
  char host_name_[NI_MAXHOST];
  char serv_name_[NI_MAXSERV];
  std::string ip_;
  std::string port_;
  struct addrinfo* inet0_; // Linux変数はC++で定義しない
  struct addrinfo* hint_;

protected: // EventLib
  std::shared_ptr<event_handler_if_t> event_handler_;
  event_callback_t event_;
};
} // namespace abc
} // ipv4
} // nw
#endif // INCLUDE_NETWORK_IPV4_ABC_SOCKET_SERVER_H
