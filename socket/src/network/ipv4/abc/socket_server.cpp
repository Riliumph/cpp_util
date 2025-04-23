#include "socket_server.h"
// STL
#include <iostream>
// POSIX Standard
#include <unistd.h>

namespace nw {
namespace ipv4 {
namespace abc {
SocketServer::SocketServer(u_short port, struct addrinfo hint)
  : server_fd_{ DISABLE_FD }
  , port_{ port }
  , inet0_{ new struct addrinfo }
  , hint_{ new struct addrinfo }
{
  Hint(hint);
}

/// @brief デストラクタ
/// @attention
/// std::unique_ptr<T>などを使う場合は、protectedではコンパイルできない。
/// publicな仮想デストラクタにしてstd::unique_ptr<T>がdeleteできるようにすること。
/// TODO:
/// 仮想デストラクタはオーバーライドの可能性があるため挙動確認とカスタムデリータの検討をすること
SocketServer::~SocketServer()
{
  SafeClose();
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

/// @brief サーバーソケットを生成する関数
/// @return 成否
int
SocketServer::CreateSocket()
{
  // addrinfo型はリンクリストを形成するため、forで対応する
  for (auto* info = inet0_; info != nullptr; info = info->ai_next) {
    server_fd_ =
      socket(inet0_->ai_family, inet0_->ai_socktype, inet0_->ai_protocol);
    if (server_fd_ < 0) {
      perror("make socket");
      continue;
    }
    // bindのチェックまでやった方がいい
  }
  std::cout << "server_fd_: " << server_fd_ << std::endl;
  return server_fd_;
}

/// @brief サーバーソケットにアドレスをバインドする
/// @return 成否
int
SocketServer::AttachAddress()
{
  auto ok = bind(server_fd_, inet0_->ai_addr, inet0_->ai_addrlen);
  if (ok < 0) {
    perror("bind");
    return ok;
  }
  return ok;
}

/// @brief サーバーのクローズ処理
void
SocketServer::SafeClose()
{
  if (0 < server_fd_) {
    close(server_fd_);
  }
  if (inet0_ != nullptr) {
    freeaddrinfo(inet0_);
  }
  if (hint_ != nullptr) {
    freeaddrinfo(hint_);
  }
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

void
SocketServer::EventHandler(std::shared_ptr<event::IF::EventHandler> eh)
{
  event_handler_ = eh;
}

/// @brief コールバックイベントの設定
/// @param e イベント
void
SocketServer::Event(event::IF::EventHandler::callback e)
{
  event_ = e;
}
} // namespace abc
} // namespace ipv4
} // namespace nw
