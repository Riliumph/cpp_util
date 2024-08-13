#include "epoll_std.h"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

// 定数定義
const int MAX_EVENTS = 10;   // 同時に処理する最大のイベント数
const int TIMEOUT_US = 1000; // epoll_waitのタイムアウト時間（μ秒）

int
main()
{
  // epollのインスタンスを作成
  int epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) {
    perror("epoll_create1");
    return 1;
  }

  // epollイベント構造体の作成
  struct epoll_event event;
  event.events = EPOLLIN;       // 読み込み可能なイベントを監視
  event.data.fd = STDIN_FILENO; // 標準入力を監視対象に設定

  // epoll_ctlで監視対象を登録
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event) == -1) {
    perror("epoll_ctl");
    close(epoll_fd);
    return 1;
  }

  // epoll_waitでイベントを待ち受ける
  while (true) {
    struct epoll_event events[MAX_EVENTS];
    int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, TIMEOUT_US);

    if (num_events == -1) {
      perror("epoll_wait");
      break;
    }

    for (int i = 0; i < num_events; ++i) {
      if (events[i].data.fd == STDIN_FILENO && events[i].events & EPOLLIN) {
        // 標準入力からデータを読み込む
        char buffer[1024];
        ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));

        if (bytes_read == -1) {
          perror("read");
          break;
        } else if (bytes_read == 0) {
          std::cout << "EOF received. Exiting..." << std::endl;
          close(epoll_fd);
          return 0;
        }

        // 読み込んだデータを標準出力に書き出す
        ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
        if (bytes_written == -1) {
          perror("write");
          break;
        }
      } else {
        // STDIN以外のイベントはここに記述する。
        // mapに<fd,std::function>の関係性を構築すれば分岐は増えないのでは？
      }
    }
  }

  close(epoll_fd);
  return 0;
}
