#ifndef INCLUDE_EVENT_INTERFACE_EVENT_HANDLER_HPP
#define INCLUDE_EVENT_INTERFACE_EVENT_HANDLER_HPP
// STL
#include <optional>
// event
#include "event/types.hpp"

namespace event {
namespace interface {
/// @brief イベントハンドラーとしてのIFクラス
class EventHandler
{
public:
  virtual bool CanReady() = 0;
  virtual int CreateTrigger(fd_t, int) = 0;
  virtual int ModifyTrigger(fd_t, int) = 0;
  virtual int DeleteTrigger(fd_t, int) = 0;
  virtual void SetCallback(fd_t, int, callback_t) = 0;
  virtual void EraseCallback(fd_t) = 0;

  virtual void RunOnce() = 0;
  virtual void Run() = 0;
  virtual void Timeout(std::optional<timeout_t>) = 0;
};
} // namespace interface
} // namespace event
#endif // INCLUDE_EVENT_INTERFACE_EVENT_HANDLER_HPP
