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
  , timeout(std::nullopt)
{
  CreateEpoll();
}

/// @brief コンストラクタ
/// @param max_event_num 監視イベント最大数
EpollHandler::EpollHandler(size_t max_event_num)
  : epoll_fd(0)
  , event_max(max_event_num)
  , events(max_event_num)
  , timeout(std::nullopt)
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
/// @param fn イベント発生時に実行するコールバック
/// @return 成否
int
EpollHandler::RegisterEvent(int fd, int event, callback fn)
{
  struct epoll_event e;
  e.data.fd = fd;
  e.events = event;
  auto ok = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &e);
  if (ok == -1) {
    perror("epoll register event");
    close(epoll_fd);
    return ok;
  }
  reaction[{ fd, event }] = fn;
  return ok;
}

/// @brief 監視しているイベントを変更する関数
/// @param fd 変更したいFD
/// @param event 変更したいイベント
/// @param fn 変更するコールバック（イベントのみ変更の場合はstd::nulloptを使用）
/// @return 成否
int
EpollHandler::ModifyEvent(int fd, int event, std::optional<callback> fn)
{
  struct epoll_event e;
  e.data.fd = fd;
  e.events = event;
  auto ok = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &e);
  if (ok == -1) {
    perror("epoll modify event");
    close(epoll_fd);
    return ok;
  }
  if (fn.has_value()) {
    reaction[{ fd, event }] = *fn;
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
  auto ok = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &e);
  if (ok == -1) {
    perror("epoll delete event");
    close(epoll_fd);
    return ok;
  }
  reaction.erase({ fd, event });
  return ok;
}

/// @brief イベントを待機する処理
/// @return 準備ができているFD数
int
EpollHandler::WaitEvent()
{
  auto to = Timeout();
  auto* head = events.data();
  auto updated_fd_num = epoll_wait(epoll_fd, head, event_max, to);
  return updated_fd_num;
}

/// @brief イベント待機のタイムアウトを設定する関数
/// @param to タイムアウト
void
EpollHandler::Timeout(std::optional<std::chrono::milliseconds> timeout)
{
  this->timeout = timeout;
}

int64_t
EpollHandler::Timeout()
{
  if (timeout.has_value()) {
    return timeout->count();
  }
  return -1;
}

/// @brief イベント監視ループ関数
/// @param fn イベント検出時に実行するコールバック
void
EpollHandler::LoopEvent()
{
  while (true) {
    events = std::vector<struct epoll_event>(event_max);
    std::cout << "wait event ..." << std::endl;
    auto updated_fd_num = WaitEvent();
    if (updated_fd_num == -1) {
      perror("epoll_wait");
      return;
    }

    for (int i = 0; i < updated_fd_num; ++i) {
      auto& event = events[i];
      // pack fieldをalignmentされた値に置き直し
      // static_cast<int>を使ってもいいが、readability-redundant-casting警告が出る
      int key_fd = event.data.fd;
      int key_ev = event.events;
      auto it = reaction.find({ key_fd, key_ev });
      if (it == reaction.end()) {
        std::cerr << "not found fd(" << event.data.fd << ")" << std::endl;
        continue;
      }
      std::cout << "callback fd(" << event.data.fd << ")" << std::endl;
      it->second(event.data.fd);
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
