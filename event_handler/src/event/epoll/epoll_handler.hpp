#ifndef INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_HPP
#define INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_HPP
// STL
#include <map>
// system
#include <sys/epoll.h>
// original
#include "event/interface.hpp"

namespace event {
/// @brief Epollを使う上での具象クラス
class EpollHandler : public event::IF::EventHandler
{
public:
  static constexpr int EVENT_MAX = 10;

  EpollHandler();
  EpollHandler(size_t);
  ~EpollHandler();

public: // EventHandler
  bool CanReady() override;
  int CreateTrigger(int, int) override;
  int ModifyTrigger(int, int) override;
  int DeleteTrigger(int, int) override;
  void SetCallback(int, int, callback) override;
  void EraseCallback(int) override;

  void RunOnce() override;
  void Run() override;
  void Timeout(std::optional<std::chrono::milliseconds>) override;

private:
  int WaitEvent() override;

  void CreateEpoll();
  int64_t Timeout();

private:
  int epoll_fd;
  size_t event_max;
  std::vector<struct epoll_event> events;
  std::optional<std::chrono::milliseconds> timeout;
  // std::map<int, std::map<int, callback>>ではなく、少し特殊な型を使ってみる
  std::map<std::pair<int, uint32_t>, callback> reaction;
};
}
#endif // INCLUDE_EVENT_EPOLL_EPOLL_HANDLER_HPP
