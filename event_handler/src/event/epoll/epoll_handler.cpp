#include "epoll_handler.hpp"
// STL
#include <iostream>
#include <vector>
// system
#include <unistd.h>
// original
#include "operator_io.hpp"

namespace event {
/// @brief デフォルトコンストラクタ
EpollHandler::EpollHandler()
  : events_(event_max_)

{
  CreateEpoll();
}

/// @brief コンストラクタ
/// @param max_event_num 監視イベント最大数
EpollHandler::EpollHandler(size_t max_event_num)
  : abc::EventHandler(max_event_num)
  , events_(event_max_)
{
  CreateEpoll();
}

/// @brief デストラクタ
EpollHandler::~EpollHandler() {}

/// @brief 監視するイベントを登録する関数
/// @param fd 新たに監視するFD
/// @param event 監視したいイベント
/// @return 0: 成功 / -1: 失敗
int
EpollHandler::CreateTrigger(int fd, int event)
{
  event_t e;
  e.data.fd = fd;
  e.events = event;
  auto ok = epoll_ctl(event_handler_fd_, EPOLL_CTL_ADD, fd, &e);
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
  event_t e;
  e.data.fd = fd;
  e.events = event;
  auto ok = epoll_ctl(event_handler_fd_, EPOLL_CTL_MOD, fd, &e);
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
  event_t e;
  e.data.fd = fd;
  e.events = event;
  auto ok = epoll_ctl(event_handler_fd_, EPOLL_CTL_DEL, fd, nullptr);
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
EpollHandler::SetCallback(int fd, int event, callback_t fn)
{
  event_t e;
  e.data.fd = fd;
  e.events = event;
  reaction_[{ fd, event }] = fn;
  std::cout << "callback set: " << e << std::endl;
}

/// @brief 指定された条件のコールバックを削除する関数
/// @param fd 削除したいコールバックに関連づくFD
void
EpollHandler::EraseCallback(int fd)
{
  std::erase_if(reaction_, [fd](auto& p) {
    auto key = p.first;
    return key.first == fd;
  });
  std::cout << "callback erased: " << fd << std::endl;
}

/// @brief イベントを待機する処理
/// @return イベントが発火したFD数
int
EpollHandler::WaitEvent()
{
  std::cout << "waiting event by epoll ..." << std::endl;
  auto to = Timeout();
  auto* head = events_.data();
  auto updated_fd_num = epoll_wait(event_handler_fd_, head, event_max_, to);
  return updated_fd_num;
}

/// @brief イベント待機のタイムアウト値を設定する関数
/// 無効値の場合は-1となり、epoll系APIにそのまま使える
/// @return タイムアウト値
int64_t
EpollHandler::Timeout()
{
  if (timeout_.has_value()) {
    return timeout_->count();
  }
  return -1;
}

/// @brief 一度だけイベントを監視する関数
void
EpollHandler::RunOnce()
{
  events_ = std::vector<event_t>(event_max_);
  auto updated_fd_num = WaitEvent();
  if (updated_fd_num == -1) {
    perror("epoll_wait");
    return;
  }

  for (int i = 0; i < updated_fd_num; ++i) {
    auto& event = events_[i];
    std::cout << "event(" << event << ") fired!!" << std::endl;
    // pack fieldをalignmentされた値に置き直し
    // static_cast<int>を使ってもいいが、readability-redundant-casting警告が出る
    int key_fd = event.data.fd;
    int key_ev = event.events;
    auto it = reaction_.find({ key_fd, key_ev });
    if (it == reaction_.end()) {
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
  event_handler_fd_ = epoll_create1(EPOLL_CLOEXEC);
  if (event_handler_fd_ == DISABLED_FD) {
    perror("epoll_create1");
  }
}
}
