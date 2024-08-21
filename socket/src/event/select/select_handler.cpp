#include "select_handler.h"
// STL
#include <chrono>
#include <iostream>
#include <vector>
// system
#include <sys/epoll.h>
#include <unistd.h>

SelectHandler::SelectHandler()
  : select_fd(0)
  , event_max(EVENT_MAX)
{
}

SelectHandler::SelectHandler(size_t max_event_num)
  : select_fd(0)
  , event_max(max_event_num)

{
}

SelectHandler::~SelectHandler()
{
  if (0 < select_fd) {
    close(select_fd);
  }
}

bool
SelectHandler::CanReady()
{
  return !(select_fd == -1);
}

int
SelectHandler::RegisterEvent(int fd, int event)
{
  switch (event) {
    case EPOLLIN:
      FD_SET(fd, &read_fds);
      break;
    case EPOLLOUT:
      FD_SET(fd, &write_fds);
      break;
    case EPOLLERR:
      FD_SET(fd, &except_fds);
      break;
    default:
      printf("unknown event: %d\n", event);
      return -1;
  }
  max_fd = GetMaxFd();
}

int
SelectHandler::ModifyEvent(int fd, int event)
{
}

int
SelectHandler::DeleteEvent(int fd, int event)
{
  FD_CLR(fd, &read_fds);
  FD_CLR(fd, &write_fds);
  FD_CLR(fd, &except_fds);
  max_fd = GetMaxFd();
}

int
SelectHandler::WaitEvent()
{

  auto updated_fd_num =
    select(max_fd + 1, &read_fds, &write_fds, &except_fds, &timeout);
  return updated_fd_num;
}

void
SelectHandler::Timeout(std::chrono::milliseconds timeout)
{
  using secs = std::chrono::seconds;
  using usecs = std::chrono::microseconds;
  auto s = std::chrono::duration_cast<secs>(timeout);
  auto us = std::chrono::duration_cast<usecs>(timeout - s);
  this->timeout.tv_sec = s.count();
  this->timeout.tv_usec = us.count();
}

void
SelectHandler::LoopEvent() {};

void
SelectHandler::LoopEvent(std::function<bool(int)> fn)
{
  while (true) {
    auto updated_event_num = WaitEvent();
    if (updated_event_num == -1) {
      perror("select_wait");
      return;
    }

    for (int i = 0; i < updated_event_num; ++i) {
      // 反応したFDがSTDIN_FILENOの場合
      if (FD_ISSET(STDIN_FILENO, &read_fds)) {
        if (!fn(STDIN_FILENO)) {
          return;
        }
      }
    }
  }
}

void
SelectHandler::CreateSelect()
{
  // SELECTにEPOLLのようなインスタンス作成は存在しない
}

int
SelectHandler::GetMaxFd()
{
  int max = 0;
  for (int i = 0; i < FD_SETSIZE; i++) {
    if (FD_ISSET(i, &read_fds) || FD_ISSET(i, &write_fds) ||
        FD_ISSET(i, &except_fds)) {
      max = std::max(max, i);
    }
  }
  return max;
}
