#include "event_handler.hpp"
// POSIX Standard
#include <unistd.h>

namespace event {
namespace abc {

EventHandler::EventHandler(size_t event_max)
  : event_max_(event_max)
{
}

EventHandler::~EventHandler()
{
  SafeClose();
}

/// @brief 実行可能を判定する関数
/// イベントハンドラインスタンス（epollやselect）が作成されていれば実行可能とみなす。
/// @return 成否
bool
EventHandler::CanReady()
{
  return event_handler_fd_ != DISABLED_FD;
}

/// @brief イベント待機のタイムアウト値を設定する関数
/// @param to タイムアウト値
void
EventHandler::Timeout(std::optional<timeout_t> timeout)
{
  timeout_ = timeout;
}

void
EventHandler::SafeClose() const
{
  if (event_handler_fd_ != DISABLED_FD) {
    close(event_handler_fd_);
  }
}

} // namespace abc
} // nanespace event
