#ifndef INCLUDE_EVENT_SELECT_HANDLER_H
#define INCLUDE_EVENT_SELECT_HANDLER_H
// STL
#include <map>
#include <vector>
// system
#include <sys/select.h>
// original
#include "event/interface.h"

namespace event {
/// @brief Epollを使う上での抽象基底クラス（インターフェイスではない）
class SelectHandler : public event::IF::EventHandler
{
public:
  static constexpr int EVENT_MAX = 10;

  SelectHandler();
  SelectHandler(size_t);
  ~SelectHandler();

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

  void CreateSelect();
  int GetMaxFd();

private:
  int select_fd;
  size_t event_max;
  int max_fd;
  fd_set read_fds;
  fd_set write_fds;
  fd_set except_fds;
  struct timeval timeout;
  std::map<int, callback> reaction;
};
}
#endif // INCLUDE_EVENT_SELECT_HANDLER_H
