// STL
#include <iostream>
// Linux
#include <unistd.h>
// original
#include "event.h"

int
main()
{
  auto e_handler = EpollHandler();
  if (!e_handler.CanReady()) {
    std::cerr << "failed to create epoll handler" << std::endl;
    return -1;
  }
  struct epoll_event e1;
  e1.events = EPOLLIN;       // 読み込み可能なイベントを監視
  e1.data.fd = STDIN_FILENO; // 標準入力を監視対象に設定
  auto ok = e_handler.RegisterEvent(e1.data.fd, e1.events);
  if (ok != 0) {
    std::cerr << "failed to register event" << std::endl;
    return -1;
  }
  std::cout << "Loop Event" << std::endl;
  e_handler.LoopEvent([](int fd) {
    std::cout << "fd: " << fd << std::endl;
    // 標準入力からデータを読み込む
    char buffer[1024];
    ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));

    if (bytes_read == -1) {
      perror("read");
      return false;
    } else if (bytes_read == 0) {
      std::cout << "EOF received. Exiting..." << std::endl;
      // close(epoll_fd);
      return false;
    }

    // 読み込んだデータを標準出力に書き出す
    ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
    if (bytes_written == -1) {
      perror("write");
      return false;
    }
    return true;
  });
  return 0;
}
