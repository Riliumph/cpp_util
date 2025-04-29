#ifndef INCLUDE_EVENT_SELECT_HANDLER_HPP
#define INCLUDE_EVENT_SELECT_HANDLER_HPP
// inherit
#include "event/abc.hpp"
// STL
#include <map>
#include <set>
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
  bool CanReady() override;
  int CreateTrigger(int, int) override;
  int ModifyTrigger(int, int) override;
  int DeleteTrigger(int, int) override;
  void SetCallback(int, int, callback_t) override;
  void EraseCallback(int) override;

  void RunOnce() override;
  void Run() override;

private:
  int WaitEvent();
  int GetMaxFd();
  struct timeval* Timeout();
  void LinkFdSet(const std::set<fd_t>&, fd_set&);

private:
  std::set<fd_t> read_fds_;
  fd_set read_mask_;
  std::set<fd_t> write_fds_;
  fd_set write_mask_;
  std::set<fd_t> except_fds_;
  fd_set except_mask_;
  std::map<fd_t, callback_t> reaction_;
};
}
#endif // INCLUDE_EVENT_SELECT_HANDLER_HPP
