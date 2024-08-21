#ifndef INCLUDE_EVENT_SELECT_SELECT_HANDLER_H
#define INCLUDE_EVENT_SELECT_SELECT_HANDLER_H
// STL
#include <chrono>
#include <vector>
// system
#include <sys/select.h>
// original
#include "event/interface.h"

/// @brief Epollを使う上での抽象基底クラス（インターフェイスではない）
class SelectHandler : public EventHandler
{
public:
  static constexpr int EVENT_MAX = 10;

  SelectHandler();
  SelectHandler(size_t);
  ~SelectHandler();

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
  void CreateSelect();
  int GetMaxFd();

protected:
  int select_fd;
  size_t event_max;
  int max_fd;
  fd_set read_fds;
  fd_set write_fds;
  fd_set except_fds;
  struct timeval timeout;
};
#endif // INCLUDE_EVENT_SELECT_SELECT_HANDLER_H
