#ifndef INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_HPP
#define INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_HPP
// inherit
#include "event/abc.hpp"
// STL
#include <map>
// system
#include <sys/epoll.h>

namespace event {
/// @brief Epollを使う具象クラス
class EpollHandler : public event::abc::EventHandler
{
  using event_t = struct epoll_event;

public:
  EpollHandler();
  EpollHandler(size_t);
  ~EpollHandler();

public: // EventHandler
  int CreateTrigger(int, int) override;
  int ModifyTrigger(int, int) override;
  int DeleteTrigger(int, int) override;
  void SetCallback(int, int, callback_t) override;
  void EraseCallback(int) override;

  void RunOnce() override;
  void Run() override;
  void Timeout(std::optional<timeout_t>) override;

private:
  int WaitEvent();

  void CreateEpoll();
  int64_t Timeout();

private:
  std::vector<event_t> events_;
  std::optional<timeout_t> timeout_;
  // std::map<int, std::map<int, callback>>ではなく、少し特殊な型を使ってみる
  std::map<std::pair<fd_t, uint32_t>, callback_t> reaction_;
};
}
#endif // INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_HPP
