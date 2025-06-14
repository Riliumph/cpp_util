#ifndef INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_HPP
#define INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_HPP
// inherit
#include "event/abc/abc.hpp"
// STL
#include <map>
#include <vector>
// event
#include "epoll_types.hpp"

namespace event {
namespace epoll {
/// @brief Epollを使う具象クラス
class EpollHandler : public event::abc::EventHandler
{
public:
  EpollHandler();
  EpollHandler(size_t);
  ~EpollHandler();

public: // EventHandler
  int CreateTrigger(fd_t, event_id_t) override;
  int ModifyTrigger(fd_t, event_id_t) override;
  int DeleteTrigger(fd_t, event_id_t) override;
  void SetCallback(fd_t, event_id_t, callback_t) override;
  void EraseCallback(fd_t) override;

  void RunOnce() override;
  void Run() override;

private:
  int WaitEvent();

  void CreateEpoll();
  int64_t Timeout();

private:
  std::vector<event_t> events_;
  // std::map<int, std::map<int, callback>>ではなく、少し特殊な型を使ってみる
  std::map<std::pair<fd_t, event_id_t>, callback_t> reaction_;
};
} // namespace epoll
} // namespace event
#endif // INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_HPP
