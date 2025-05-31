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
namespace select {
/// @brief Epollを使う具象クラス
class SelectHandler : public event::abc::EventHandler
{
public:
  SelectHandler();
  SelectHandler(size_t);
  ~SelectHandler();

public: // EventHandler
  bool CanReady() override;
  int CreateTrigger(fd_t, event_id_t) override;
  int ModifyTrigger(fd_t, event_id_t) override;
  int DeleteTrigger(fd_t, event_id_t) override;
  void SetCallback(fd_t, event_id_t, callback_t) override;
  void EraseCallback(fd_t) override;

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
} // namespace select
} // namespace event
#endif // INCLUDE_EVENT_SELECT_HANDLER_HPP
