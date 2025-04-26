#ifndef INCLUDE_EVENT_ABC_EVENT_HANDLER_HPP
#define INCLUDE_EVENT_ABC_EVENT_HANDLER_HPP
// inherit
#include "event/interface.hpp"
// STL
#include <chrono>
#include <map>
#include <optional>

namespace event {
namespace abc {
class EventHandler : public event::IF::EventHandler
{
public:
  static constexpr int DISABLED_FD = -1;
  static constexpr int EVENT_MAX = 10;

protected:
  EventHandler();
  EventHandler(size_t);
  ~EventHandler();

public:
  virtual bool CanReady() override;

private:
  void SafeClose() const;

protected:
  int event_handler_fd_;
  size_t event_max_;
};
}
}
#endif // INCLUDE_EVENT_ABC_EVENT_HANDLER_HPP
