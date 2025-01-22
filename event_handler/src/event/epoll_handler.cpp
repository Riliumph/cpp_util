#include "epoll_handler.h"
// STL
#include <iostream>
#include <vector>
// system
#include <unistd.h>
// original
#include "event/operator_io.hpp"

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
/// epollインスタンスが作成されていれば実行可能とみなす。
/// @return 成否
bool
EpollHandler::CanReady()
{
  return epoll_fd != -1;
}

/// @brief 監視するイベントを登録する関数
/// @param fd 新たに監視するFD
/// @param event 監視したいイベント
/// @return 0: 成功 / -1: 失敗
int
EpollHandler::CreateTrigger(int fd, int event)
{
  struct epoll_event e;
  e.data.fd = fd;
  e.events = event;
  auto ok = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &e);
  if (ok == -1) {
    perror("failed create epoll trigger");
    return ok;
  }
  std::cout << "trigger created: " << e << std::endl;
  return ok;
}

/// @brief 監視しているイベントを変更する関数
/// @param fd 変更したいFD
/// @param event 変更したいイベント
/// @return 0: 成功 / -1: 失敗
int
EpollHandler::ModifyTrigger(int fd, int event)
{
  struct epoll_event e;
  e.data.fd = fd;
  e.events = event;
  auto ok = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &e);
  if (ok == -1) {
    perror("failed modify epoll trigger");
    return ok;
  }
  std::cout << "trigger modified: " << e << std::endl;
  return ok;
}

/// @brief 監視しているイベントを削除する関数
/// @param fd 削除したいFD
/// @param event 削除したいイベント（使われない）
/// @return 0: 成功 / -1: 失敗
int
EpollHandler::DeleteTrigger(int fd, int event)
{
  struct epoll_event e;
  e.data.fd = fd;
  e.events = event;
  auto ok = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
  if (ok == -1) {
    perror("failed delete epoll trigger");
    return ok;
  }
  std::cout << "trigger deleted: " << e << std::endl;
  return ok;
}

/// @brief 発火したイベントに対応するコールバックを設定する関数
/// @param fd 発火したFD
/// @param event 発火したイベント
/// @param fn 設定するコールバック
void
EpollHandler::SetCallback(int fd, int event, callback fn)
{
  struct epoll_event e;
  e.data.fd = fd;
  e.events = event;
  reaction[{ fd, event }] = fn;
  std::cout << "callback set: " << e << std::endl;
}

/// @brief 指定された条件のコールバックを削除する関数
/// @param fd 削除したいコールバックに関連づくFD
void
EpollHandler::EraseCallback(int fd)
{
  std::erase_if(reaction, [fd](auto& p) { return p.first.first == fd; });
  std::cout << "callback erased: " << fd << std::endl;
}

/// @brief イベントを待機する処理
/// @return イベントが発火したFD数
int
EpollHandler::WaitEvent()
{
  std::cout << "waiting event ..." << std::endl;
  auto to = Timeout();
  auto* head = events.data();
  auto updated_fd_num = epoll_wait(epoll_fd, head, event_max, to);
  return updated_fd_num;
}

/// @brief イベント待機のタイムアウト値を設定する関数
/// @param to タイムアウト値
void
EpollHandler::Timeout(std::optional<std::chrono::milliseconds> timeout)
{
  this->timeout = timeout;
}

/// @brief イベント待機のタイムアウト値を設定する関数
/// 無効値の場合は-1となり、epoll系APIにそのまま使える
/// @return タイムアウト値
int64_t
EpollHandler::Timeout()
{
  if (timeout.has_value()) {
    return timeout->count();
  }
  return -1;
}

/// @brief 一度だけイベントを監視する関数
void
EpollHandler::RunOnce()
{
  events = std::vector<struct epoll_event>(event_max);
  auto updated_fd_num = WaitEvent();
  if (updated_fd_num == -1) {
    perror("epoll_wait");
    return;
  }

  for (int i = 0; i < updated_fd_num; ++i) {
    auto& event = events[i];
    std::cout << "event(" << event << ") fired!!" << std::endl;
    // pack fieldをalignmentされた値に置き直し
    // static_cast<int>を使ってもいいが、readability-redundant-casting警告が出る
    int key_fd = event.data.fd;
    int key_ev = event.events;
    auto it = reaction.find({ key_fd, key_ev });
    if (it == reaction.end()) {
      std::cerr << "callback not found" << std::endl;
      continue;
    }
    it->second(event.data.fd);
  }
}

/// @brief イベントを無限に監視する関数
void
EpollHandler::Run()
{
  while (true) {
    RunOnce();
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
