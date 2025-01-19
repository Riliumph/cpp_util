// STL
#include <iostream>
// Linux
#include <unistd.h>
// original
#include "event.h"

void
read_and_print(int fd)
{
  // 標準入力からデータを読み込む
  char buffer[1024];
  auto bytes_read = read(fd, buffer, sizeof(buffer));

  if (bytes_read == -1) {
    perror("read");
    return;
  } else if (bytes_read == 0) {
    std::cout << "EOF received. Exiting..." << std::endl;
    // close(epoll_fd);
    return;
  }

  // 読み込んだデータを標準出力に書き出す
  auto bytes_written = write(fd, buffer, bytes_read);
  if (bytes_written == -1) {
    perror("write");
    return;
  }
  // 次のreturnは現状readability-redundant-control-flowを引き起こす
  // ただし、上記if内のreturnは異常系の早期return
  // 下記のreturnは正常系のreturnなので分けておくのが適切
  // そのため、コメントによりlintを無効化する
  // NOLINTNEXTLINE(readability-redundant-control-flow)
  return;
}

int
main()
{
  auto e_handler = event::EpollHandler();
  if (!e_handler.CanReady()) {
    std::cerr << "failed to create epoll handler" << std::endl;
    return -1;
  }
  struct epoll_event e1;
  e1.events = EPOLLIN;       // 読み込み可能なイベントを監視
  e1.data.fd = STDIN_FILENO; // 標準入力を監視対象に設定
  auto ok = e_handler.RegisterEvent(e1.data.fd, e1.events, read_and_print);
  if (ok != 0) {
    std::cerr << "failed to register event" << std::endl;
    return -1;
  }
  e_handler.LoopEvent();
  return 0;
}
