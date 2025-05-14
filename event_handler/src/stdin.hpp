#ifndef INCLUDE_STDIN_HPP
#define INCLUDE_STDIN_HPP
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

void
stdin_sample(std::shared_ptr<event::interface::EventHandler> e_handler)
{
  struct epoll_event e;
  e.events = EPOLLIN;       // 読み込み可能なイベントを監視
  e.data.fd = STDIN_FILENO; // 標準入力を監視対象に設定
  auto ok = e_handler->CreateTrigger(e.data.fd, e.events);
  if (ok != 0) {
    std::cerr << "failed to register event" << std::endl;
    exit(1);
  }
  e_handler->SetCallback(e.data.fd, EPOLLIN, [](int fd) {
    std::printf("===== RECEIVED EVENT(%d) =====\n", fd);
    // 標準入力からデータを読み込む
    char buffer[1024];
    auto bytes_read = read(fd, buffer, sizeof(buffer));

    if (bytes_read == -1) {
      perror("read");
      exit(1);
    } else if (bytes_read == 0) {
      std::cout << "EOF received. Exiting..." << std::endl;
      // close(epoll_fd);
      return;
    }

    // 読み込んだデータを標準出力に書き出す
    auto bytes_written = write(fd, buffer, bytes_read);
    if (bytes_written == -1) {
      perror("write");
      exit(1);
    }
    return;
  });
}
#endif // INCLUDE_STDIN_HPP
