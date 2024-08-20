#include "epoll_handler.h"
// STL
#include <iostream>
#include <vector>
// system
#include <unistd.h>

EpollHandler::EpollHandler()
  : epoll_fd(0)
  , event_max(EVENT_MAX)
  , events(EVENT_MAX)
{
  CreateEpoll();
}

EpollHandler::EpollHandler(size_t max_event_num)
  : epoll_fd(0)
  , event_max(max_event_num)
  , events(max_event_num)

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
  auto to = timeout.count();
  auto updated_event_num = epoll_wait(epoll_fd, events.data(), event_max, to);
  return updated_event_num;
}

void
EpollHandler::Timeout(std::chrono::milliseconds to)
{
  timeout = to;
}

void
EpollHandler::LoopEvent() {};

void
EpollHandler::LoopEvent(std::function<bool(int)> fn)
{
  while (true) {
    events = std::vector<struct epoll_event>(event_max);
    auto updated_event_num = WaitEvent();
    if (updated_event_num == -1) {
      perror("epoll_wait");
      return;
    }

    for (int i = 0; i < updated_event_num; ++i) {
      if (events[i].data.fd == STDIN_FILENO && events[i].events & EPOLLIN) {
        // 反応したFDがSTDIN_FILENOの場合
        if (!fn(events[i].data.fd)) {
          return;
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
