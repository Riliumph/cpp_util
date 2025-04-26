#ifndef INCLUDE_EVENT_SELECT_HANDLER_HPP
#define INCLUDE_EVENT_SELECT_HANDLER_HPP
// inherit
#include "event/abc.hpp"
// STL
#include <map>
// system
#include <sys/select.h>

namespace event {
/// @brief Epollを使う上での抽象基底クラス（インターフェイスではない）
class SelectHandler : public event::abc::EventHandler
{
public:
  SelectHandler();
  SelectHandler(size_t);
  ~SelectHandler();

public: // EventHandler
  bool CanReady() override;
  int CreateTrigger(int, int) override;
  int ModifyTrigger(int, int) override;
  int DeleteTrigger(int, int) override;
  void SetCallback(int, int, callback_t) override;
  void EraseCallback(int) override;
  void RunOnce() override;
  void Run() override;
  void Timeout(std::optional<timeout_t>) override;

private:
  int WaitEvent() override;
  int GetMaxFd();

private:
  int max_fd_;
  fd_set read_fds_;
  fd_set write_fds_;
  fd_set except_fds_;
  struct timeval timeout_;
  std::map<int, callback_t> reaction_;
};
}
#endif // INCLUDE_EVENT_SELECT_HANDLER_HPP
