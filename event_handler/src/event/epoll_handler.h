#ifndef INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_H
#define INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_H
// STL
#include <map>
// system
#include <sys/epoll.h>
// original
#include "event/interface.h"

namespace event {
/// @brief Epollを使う上での抽象基底クラス（インターフェイスではない）
class EpollHandler : public event::IF::EventHandler
{
public:
  static constexpr int EVENT_MAX = 10;

  EpollHandler();
  EpollHandler(size_t);
  ~EpollHandler();

public: // EventHandler
  bool CanReady() override;
  int RegisterEvent(int, int, event_func) override;
  int ModifyEvent(int, int, std::optional<event_func> = std::nullopt) override;
  int DeleteEvent(int, int) override;
  void LoopEvent() override;
  void Timeout(std::chrono::milliseconds) override;

private:
  int WaitEvent() override;

  void CreateEpoll();

private:
  int epoll_fd;
  size_t event_max;
  std::vector<struct epoll_event> events;
  std::chrono::milliseconds timeout;
  // TODO: fdのみをキーとする。今後イベントの種類にも対応
  std::map<int, event_func> reaction;
};
}
#endif // INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_H
