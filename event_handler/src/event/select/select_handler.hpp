#ifndef INCLUDE_EVENT_SELECT_HANDLER_HPP
#define INCLUDE_EVENT_SELECT_HANDLER_HPP
// inherit
#include "event/abc.hpp"
// STL
#include <map>
// system
#include <sys/select.h>

namespace event {
/// @brief Epollを使う具象クラス
class SelectHandler : public event::abc::EventHandler
{
public:
  SelectHandler();
  SelectHandler(size_t);
  ~SelectHandler();

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
  int GetMaxFd();

private:
  fd_t max_fd_;
  fd_set read_fds_;
  fd_set write_fds_;
  fd_set except_fds_;
  struct timeval timeout_;
  std::map<fd_t, callback_t> reaction_;
};
}
#endif // INCLUDE_EVENT_SELECT_HANDLER_HPP
