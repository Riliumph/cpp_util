#ifndef INCLUDE_EVENT_INTERFACE_EVENT_HANDLER_H
#define INCLUDE_EVENT_INTERFACE_EVENT_HANDLER_H
// STL
#include <chrono>
#include <functional>

/// @brief イベントハンドラーとしてのIFクラス
class EventHandler
{
public:
  virtual int WaitEvent() = 0;
  virtual void LoopEvent(std::function<bool(int)>) = 0;
  virtual void LoopEvent() = 0;
  virtual void Timeout(std::chrono::milliseconds to) = 0;
};
#endif // INCLUDE_EVENT_INTERFACE_EVENT_HANDLER_H
