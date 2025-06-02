#ifndef INCLUDE_EVENT_ABC_EVENT_HANDLER_HPP
#define INCLUDE_EVENT_ABC_EVENT_HANDLER_HPP
// inherit
#include "event/interface/interface.hpp"
// STL
#include <optional>

namespace event {
namespace abc {
class EventHandler : public event::interface::EventHandler
{
protected:
  static constexpr int EVENT_MAX = 10;

protected:
  EventHandler() = default;
  EventHandler(size_t);
  ~EventHandler();

public:
  virtual bool CanReady() override;
  virtual void Timeout(std::optional<timeout_t>) override;

private:
  void SafeClose() const;

protected:
  fd_t event_handler_fd_ = DISABLED_FD;
  size_t event_max_ = EVENT_MAX;
  std::optional<timeout_t> timeout_ = std::nullopt;
};
} // namespace abc
} // namespace event
#endif // INCLUDE_EVENT_ABC_EVENT_HANDLER_HPP
