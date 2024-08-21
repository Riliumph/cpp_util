#include "epoll_handler.h"
// STL
#include <iostream>
#include <vector>
// system
#include <unistd.h>

namespace event {
/// @brief デフォルトコンストラクタ
EpollHandler::EpollHandler()
  : epoll_fd(0)
  , event_max(EVENT_MAX)
  , events(EVENT_MAX)
{
  CreateEpoll();
}

/// @brief コンストラクタ
/// @param max_event_num 監視イベント最大数
EpollHandler::EpollHandler(size_t max_event_num)
  : epoll_fd(0)
  , event_max(max_event_num)
  , events(max_event_num)

{
  CreateEpoll();
}

/// @brief デストラクタ
EpollHandler::~EpollHandler()
{
  if (0 < epoll_fd) {
    close(epoll_fd);
  }
}

/// @brief 実行可能を判定する関数
/// @return 成否
bool
EpollHandler::CanReady()
{
  return epoll_fd != -1;
}

/// @brief 監視するイベントを登録する関数
/// @param fd 新たに監視するFD
/// @param event 監視したいイベント
/// @return 成否
int
EpollHandler::RegisterEvent(int fd, int event)
{
  struct epoll_event e;
  e.data.fd = fd;
  e.events = event;
  auto ok = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, e.data.fd, &e);
  if (ok == -1) {
    perror("epoll register event");
    close(epoll_fd);
    return ok;
  }
  return ok;
}

/// @brief 監視しているイベントを変更する関数
/// @param fd 変更したいFD
/// @param event 変更したいイベント
/// @return 成否
int
EpollHandler::ModifyEvent(int fd, int event)
{
  struct epoll_event e;
  e.data.fd = fd;
  e.events = event;
  auto ok = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, e.data.fd, &e);
  if (ok == -1) {
    perror("epoll modify event");
    close(epoll_fd);
    return ok;
  }
  return ok;
}

/// @brief 監視しているイベントを削除する関数
/// @param fd 削除したいFD
/// @param event 削除したいイベント
/// @return 成否
int
EpollHandler::DeleteEvent(int fd, int event)
{
  struct epoll_event e;
  e.data.fd = fd;
  e.events = event;
  auto ok = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, e.data.fd, &e);
  if (ok == -1) {
    perror("epoll delete event");
    close(epoll_fd);
    return ok;
  }
  return ok;
}

/// @brief イベントを待機する処理
/// @return 準備ができているFD数
int
EpollHandler::WaitEvent()
{
  auto to = timeout.count();
  auto updated_fd_num = epoll_wait(epoll_fd, events.data(), event_max, to);
  return updated_fd_num;
}

/// @brief イベント待機のタイムアウトを設定する関数
/// @param to タイムアウト
void
EpollHandler::Timeout(std::chrono::milliseconds timeout)
{
  this->timeout = timeout;
}

/// @brief イベント監視ループ関数
/// @param fn イベント検出時に実行するコールバック
void
EpollHandler::LoopEvent(std::function<bool(int)> fn)
{
  while (true) {
    events = std::vector<struct epoll_event>(event_max);
    auto updated_fd_num = WaitEvent();
    if (updated_fd_num == -1) {
      perror("epoll_wait");
      return;
    }

    for (int i = 0; i < updated_fd_num; ++i) {
      if (events[i].data.fd == STDIN_FILENO && events[i].events & EPOLLIN) {
        // 反応したFDがSTDIN_FILENOの場合
        if (!fn(events[i].data.fd)) {
          return;
        }
      }
    }
  }
}

/// @brief Epollインスタンスを作成する関数
void
EpollHandler::CreateEpoll()
{
  epoll_fd = epoll_create1(EPOLL_CLOEXEC);
  if (epoll_fd == -1) {
    perror("epoll_create1");
  }
}
}
