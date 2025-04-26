#ifndef INCLUDE_SOCKET_HPP
#define INCLUDE_SOCKET_HPP
// STL
#include <iostream>
#include <memory>
// Linux
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
// original
#include "event.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>

constexpr int MAX_EVENTS = 10;
constexpr int PORT = 50000;
constexpr int BUF_SIZE = 1024;

// コールバック関数の型
using Callback = std::function<void(int)>;

void
socket_sample(std::shared_ptr<event::interface::EventHandler> e_handler)
{
  // ソケットの作成
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("Socket creation failed");
    exit(1);
  }

  // ソケットを非ブロッキングモードに設定
  //   set_nonblocking(server_fd);

  // サーバーアドレスの設定
  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  // バインド
  int ok = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (ok == -1) {
    perror("Bind failed");
    close(server_fd);
    exit(1);
  }
  std::cout << "server_fd: " << server_fd << std::endl;

  // リスニング
  ok = listen(server_fd, 10);
  if (ok == -1) {
    perror("Listen failed");
    close(server_fd);
    exit(1);
  }

  struct epoll_event e;
  e.data.fd = server_fd;
  e.events = EPOLLIN;
  ok = e_handler->CreateTrigger(e.data.fd, e.events);
  if (ok == -1) {
    perror("trigger create failed");
    exit(1);
  }
  e_handler->SetCallback(e.data.fd, EPOLLIN, [e_handler](int server_fd) {
    std::cout << "callback: " << server_fd << std::endl;
    // 新しい接続の受け入れ
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    int client_fd =
      accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
      perror("Accept failed");
      exit(1);
    }
    std::cout << "client_fd: " << client_fd << std::endl;

    // 新しいクライアントを非ブロッキングモードに設定
    // set_nonblocking(client_fd);

    // クライアントソケットをepollに登録
    struct epoll_event e;
    e.events = (EPOLLIN | EPOLLRDHUP);
    e.data.fd = client_fd;
    std::cout << "create trigger for client" << std::endl;
    e_handler->CreateTrigger(e.data.fd, e.events);
    e_handler->SetCallback(e.data.fd, EPOLLIN, [&](int fd) {
      char buffer[BUF_SIZE];
      ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
      if (bytes_read == -1) {
        perror("Read failed");
        exit(1);
      }
      if (bytes_read == 0) {
        // クライアントが切断した場合
        std::cout << "read 0 byte" << std::endl;
      } else {
        // 受信したデータを表示
        buffer[bytes_read] = '\0'; // Null terminate
        std::cout << "Received from client " << fd << ": " << buffer
                  << std::endl;
      }
    });

    // バッファに改行コードが残っていると、両方のフラグが立って同時にイベント発火する
    // telnetで切断（`^]'とctrl+d）すると両方が飛んでくる。
    e_handler->SetCallback(e.data.fd, (EPOLLIN | EPOLLRDHUP), [&](int fd) {
      std::cout << "Client disconnected: " << fd << std::endl;
      e_handler->DeleteTrigger(fd, 0);
      e_handler->EraseCallback(fd);
      close(fd);
    });
    std::cout << "New connection accepted: " << client_fd << std::endl;
  });
}
#endif // INCLUDE_SOCKET_HPP
