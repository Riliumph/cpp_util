#include "socket_server.h"
// STL
#include <iostream>

namespace nw {
namespace ipv4 {
namespace abc {
SocketServer::SocketServer(u_short port, struct addrinfo hint)
  : server_fd_{ 0 }
  , port_{ port }
  , inet0_{ new struct addrinfo }
  , hint_{ new struct addrinfo }
{
  Hint(hint);
}

/// @brief ヒント情報からアドレス情報を決定する
/// @param hint 不完全なaddrinfo構造体
/// @param service_name サービス名
/// Linux: /etc/services
/// Windows: C:\Windows\system32\drivers\etc\services
/// @return
int
SocketServer::Identify(std::string service_name)
{
  if (service_name.empty()) {
    service_name = std::to_string(port_);
  }
  // ヒント変数からアドレスを決定し、inet0変数へ設定
  // docに解説有り
  auto err = getaddrinfo(NULL, service_name.data(), hint_, &inet0_);
  if (err != 0) {
    std::cerr << "getaddrinfo(): " << gai_strerror(err) << std::endl;
    return -1;
  }

  err = getnameinfo(inet0_->ai_addr,
                    inet0_->ai_addrlen,
                    host_name_, // output引数
                    sizeof(host_name_),
                    serv_name_, // output引数
                    sizeof(serv_name_),
                    NI_NUMERICHOST | NI_NUMERICSERV);
  if (err != 0) {
    std::cerr << "getnameinfo(): " << gai_strerror(err) << std::endl;
    freeaddrinfo(inet0_);
    return -1;
  }
  printf("identify: %s:%s\n", host_name_, serv_name_);
  return 0;
}

/// @brief サーバーを構築するネットワーク情報のヒントを設定する
/// @param hint ヒント
void
SocketServer::Hint(const struct addrinfo& hint_data)
{
  hint_->ai_family = hint_data.ai_family;
  hint_->ai_socktype = hint_data.ai_socktype;
  hint_->ai_flags = hint_data.ai_flags;
}

} // namespace abc
} // namespace ipv4
} // namespace nw
