#ifndef INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_H
#define INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_H
// STL
#include <chrono>
// system
#include <sys/epoll.h>
// original
#include "event/interface.h"

/// @brief Epollを使う上での抽象基底クラス（インターフェイスではない）
class EpollHandler : public EventHandler
{
public:
  static constexpr int EVENT_MAX = 10;

  EpollHandler();
  EpollHandler(size_t);
  ~EpollHandler();

public: // EventHandler
  bool CanReady();
  int RegisterEvent(int fd, int event);
  int ModifyEvent(int fd, int event);
  int DeleteEvent(int fd, int event);
  int WaitEvent() override;
  void LoopEvent(std::function<bool(int)>) override;
  void LoopEvent() override;
  void Timeout(std::chrono::milliseconds) override;

protected:
  void CreateEpoll();

protected:
  int epoll_fd;
  size_t event_max;
  std::vector<struct epoll_event> events;
  std::chrono::milliseconds timeout;
};
#endif // INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_H
