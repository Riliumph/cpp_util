#ifndef INCLUDE_EPOLL_STD_H
#define INCLUDE_EPOLL_STD_H
// original
#include "event.h"

class StdEventHandler : public EpollHandler
{
public:
  StdEventHandler();
  StdEventHandler(int max_event);
  ~StdEventHandler();

public: // EventHandler
  void LoopEvent(std::function<void(int)> fn) override;
};

#endif // INCLUDE_EPOLL_STD_H
