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
  using callback = std::function<void(int)>;

  virtual bool CanReady() = 0;
  virtual int RegisterEvent(int, int, callback) = 0;
  virtual int ModifyEvent(int, int, std::optional<callback>) = 0;
  virtual int DeleteEvent(int, int) = 0;

  virtual void RunOnce() = 0;
  virtual void Run() = 0;
  virtual void Timeout(std::optional<std::chrono::milliseconds>) = 0;

protected:
  virtual int WaitEvent() = 0;
};
}
#endif // INCLUDE_EVENT_INTERFACE_EVENT_HANDLER_H
