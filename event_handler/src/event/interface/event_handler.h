#ifndef INCLUDE_EVENT_INTERFACE_EVENT_HANDLER_H
#define INCLUDE_EVENT_INTERFACE_EVENT_HANDLER_H
// STL
#include <chrono>
#include <functional>
#include <optional>

namespace event::IF {
/// @brief イベントハンドラーとしてのIFクラス
class EventHandler
{
public:
  using event_func = std::function<void(int)>;

  virtual bool CanReady() = 0;
  virtual int RegisterEvent(int, int, event_func) = 0;
  virtual int ModifyEvent(int, int, std::optional<event_func>) = 0;
  virtual int DeleteEvent(int, int) = 0;

  virtual void LoopEvent() = 0;
  virtual void Timeout(std::chrono::milliseconds to) = 0;

protected:
  virtual int WaitEvent() = 0;
};
}
#endif // INCLUDE_EVENT_INTERFACE_EVENT_HANDLER_H
