#ifndef INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_H
#define INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_H
// STL
#include <chrono>
#include <functional>
// system
#include <sys/epoll.h>
// original
#include "event/interface.h"

/// @brief Epollを使う上での抽象基底クラス（インターフェイスではない）
class EpollHandler : public EventHandler
{
public:
  EpollHandler();
  EpollHandler(int max_event);
  ~EpollHandler();

  bool CanReady();
  int RegisterEvent(struct epoll_event e);
  int ModifyEvent(struct epoll_event e);
  int DeleteEvent(struct epoll_event e);

public: // EventHandler
  int WaitEvent() override;
  void LoopEvent() override;
  void Timeout(std::chrono::milliseconds to) override;

protected:
  void CreateEpoll();

protected:
  int epoll_fd;
  int max_event_num;
  std::chrono::milliseconds timeout;
};
#endif // INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_H
