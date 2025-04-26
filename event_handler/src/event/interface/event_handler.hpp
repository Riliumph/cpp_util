#ifndef INCLUDE_EVENT_INTERFACE_EVENT_HANDLER_HPP
#define INCLUDE_EVENT_INTERFACE_EVENT_HANDLER_HPP
// STL
#include <chrono>
#include <functional>
#include <optional>

namespace event {
namespace interface {
/// @brief イベントハンドラーとしてのIFクラス
class EventHandler
{
public:
  using callback_t = std::function<void(int)>;
  using timeout_t = std::chrono::milliseconds;

  virtual bool CanReady() = 0;
  virtual int CreateTrigger(int, int) = 0;
  virtual int ModifyTrigger(int, int) = 0;
  virtual int DeleteTrigger(int, int) = 0;
  virtual void SetCallback(int, int, callback_t) = 0;
  virtual void EraseCallback(int) = 0;

  virtual void RunOnce() = 0;
  virtual void Run() = 0;
  virtual void Timeout(std::optional<timeout_t>) = 0;
};
} // namespace interface
} // namespace event
#endif // INCLUDE_EVENT_INTERFACE_EVENT_HANDLER_HPP
