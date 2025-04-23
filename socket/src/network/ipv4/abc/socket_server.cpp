#include "socket_server.h"

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
