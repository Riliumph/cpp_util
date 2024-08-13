#include "std_event_handler.h"
// STL
#include <iostream>
// system
#include <unistd.h>

StdEventHandler::StdEventHandler() {}

StdEventHandler::~StdEventHandler() {}

void
StdEventHandler::LoopEvent(std::function<void(int)> fn)
{
  int to = timeout.count();
  while (true) {
    struct epoll_event events[max_event_num];
    auto updated_event_num = epoll_wait(epoll_fd, events, max_event_num, to);

    if (updated_event_num == -1) {
      perror("epoll_wait");
      return;
    }

    for (size_t i = 0; i < updated_event_num; ++i) {
      if (events[i].data.fd == STDIN_FILENO && events[i].events & EPOLLIN) {
        fn(events[i].data.fd);
        // 標準入力からデータを読み込む
        char buffer[1024];
        ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));

        if (bytes_read == -1) {
          perror("read");
          break;
        } else if (bytes_read == 0) {
          std::cout << "EOF received. Exiting..." << std::endl;
          close(epoll_fd);
          return;
        }

        // 読み込んだデータを標準出力に書き出す
        ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
        if (bytes_written == -1) {
          perror("write");
          break;
        }
      }
    }
  }
}
