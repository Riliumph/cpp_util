#ifndef INCLUDE_EVENT_ABC_EVENT_HANDLER_HPP
#define INCLUDE_EVENT_ABC_EVENT_HANDLER_HPP
// inherit
#include "event/interface.hpp"
// STL
#include <chrono>
#include <map>
#include <optional>
// original
#include "types.hpp"

namespace event {
namespace abc {
class EventHandler : public event::interface::EventHandler
{
protected:
  static constexpr int EVENT_MAX = 10;

protected:
  EventHandler();
  EventHandler(size_t);
  ~EventHandler();

public:
  virtual bool CanReady() override;
  virtual void Timeout(std::optional<timeout_t>) override;

private:
  void SafeClose() const;

protected:
  fd_t event_handler_fd_;
  size_t event_max_;
  std::optional<timeout_t> timeout_;
};
} // namespace abc
} // namespace event
#endif // INCLUDE_EVENT_ABC_EVENT_HANDLER_HPP
