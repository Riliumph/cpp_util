// stl
#include <vector>
// system
#include <sys/socket.h>
#include <sys/types.h> // 使用しないがBSDなどで要求される可能性あり
#include <sys/un.h>
// original
#include "tcp/server/server.hpp"

namespace tcp {
#ifndef CONSTRUCTOR
Server::Server(std::unique_ptr<SocketIF> socket)
  : server{ std::move(socket) }
{
}

Server::Server(std::unique_ptr<tcp::SocketIF> socket)
  : server{ std::move(socket) }
{
}

Server::~Server() {}
#endif // CONSTRUCTOR

int
Server::Listen()
{
  return listen(server->FD(), MAX_BACKLOG);
}

int
Server::Accept()
{
  auto client = std::make_unique<typeid(server.get()).name()>();
  sockaddr_un client_name;
  socklen_t client_name_len;
  int client_socket_fd;
  /* 接続があるまで待機 */
  client_socket_fd = accept(
    server->FD(), reinterpret_cast<sockaddr*>(&client_name), &client_name_len);
}

int
Server::Receive(std::vector<char> buffer)
{
  return recv(client->FD(), buffer.data(), buffer.size(), 0);
}

}
