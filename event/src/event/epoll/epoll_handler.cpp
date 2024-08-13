#include "epoll_handler.h"
// STL
#include <iostream>
// system
#include <unistd.h>

EpollHandler::EpollHandler()
  : epoll_fd(0)
  , max_event_num(10)
{
  CreateEpoll();
}

EpollHandler::EpollHandler(int max_event_num)
  : epoll_fd(0)
  , max_event_num(max_event_num)
{
  CreateEpoll();
}

EpollHandler::~EpollHandler()
{
  if (0 < epoll_fd) {
    close(epoll_fd);
  }
}

bool
EpollHandler::CanReady()
{
  return !(epoll_fd == -1);
}

int
EpollHandler::RegisterEvent(struct epoll_event e)
{
  auto ok = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, e.data.fd, &e);
  if (ok == -1) {
    perror("epoll register event");
    close(epoll_fd);
    return ok;
  }
  return ok;
}

int
EpollHandler::ModifyEvent(struct epoll_event e)
{
  auto ok = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, e.data.fd, &e);
  if (ok == -1) {
    perror("epoll modify event");
    close(epoll_fd);
    return ok;
  }
  return ok;
}

int
EpollHandler::DeleteEvent(struct epoll_event e)
{
  auto ok = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, e.data.fd, &e);
  if (ok == -1) {
    perror("epoll delete event");
    close(epoll_fd);
    return ok;
  }
  return ok;
}

int
EpollHandler::WaitEvent()
{
  auto updated_event_num = epoll_wait(epoll_fd, events, max_event_num, to);
}

void
EpollHandler::Timeout(std::chrono::milliseconds to)
{
  timeout = to;
}

void
EpollHandler::LoopEvent()
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

void
EpollHandler::CreateEpoll()
{
  epoll_fd = epoll_create1(EPOLL_CLOEXEC);
  if (epoll_fd == -1) {
    perror("epoll_create1");
  }
}
