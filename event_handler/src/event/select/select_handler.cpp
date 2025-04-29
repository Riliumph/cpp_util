#include "select_handler.hpp"
// STL
#include <iostream>
#include <vector>
// system
#include <sys/epoll.h>
#include <unistd.h>
// original
#include "operator_io.hpp"

namespace event {
/// @brief デフォルトコンストラクタ
SelectHandler::SelectHandler()
{
  FD_ZERO(&read_mask_);
  FD_ZERO(&write_mask_);
  FD_ZERO(&except_mask_);
}

/// @brief コンストラクタ
/// @param max_event_num 監視イベント最大数
SelectHandler::SelectHandler(size_t max_event_num)
  : abc::EventHandler(max_event_num)
{
  FD_ZERO(&read_mask_);
  FD_ZERO(&write_mask_);
  FD_ZERO(&except_mask_);
}

/// @brief デストラクタ
SelectHandler::~SelectHandler() {}

/// @brief 実行可能を判定する関数
/// @details selectはepollと違ってインスタンス準備フェイズがない。
/// select()のタイミングですべてのエラーを検知するため必ず実行可能になる
/// @return 成否
bool
SelectHandler::CanReady()
{
  return true;
}

/// @brief 監視するイベントを登録する関数
/// @param fd 新たに監視するFD
/// @param event 監視したいイベント
/// @return 成否
int
SelectHandler::CreateTrigger(int fd, int event)
{
  if (fd == DISABLED_FD) {
    return -1;
  }
  std::cout << "create trigger for " << fd << "..." << std::endl;
  if (static_cast<bool>(event & EPOLLIN)) {
    read_fds_.insert(fd);
    std::cout << "set write trigger" << std::endl;
  }
  if (static_cast<bool>(event & EPOLLOUT)) {
    write_fds_.insert(fd);
    std::cout << "set read trigger" << std::endl;
  }
  if (static_cast<bool>(event & EPOLLERR)) {
    except_fds_.insert(fd);
    std::cout << "set except trigger" << std::endl;
  }
  return 0;
}

/// @brief 監視しているイベントを変更する関数
/// @param fd 変更したいFD
/// @param event 変更したいイベント
/// @return 成否
int
SelectHandler::ModifyTrigger(int fd, int event)
{
  if (fd == DISABLED_FD) {
    return -1;
  }
  if (static_cast<bool>(event & EPOLLIN)) {
    read_fds_.insert(fd);
  } else {
    read_fds_.erase(fd);
  }
  if (static_cast<bool>(event & EPOLLOUT)) {
    write_fds_.insert(fd);
  } else {
    write_fds_.erase(fd);
  }
  if (static_cast<bool>(event & EPOLLERR)) {
    except_fds_.insert(fd);
  } else {
    except_fds_.erase(fd);
  }
  return 0;
}

/// @brief 監視しているイベントを削除する関数
/// @param fd 削除したいFD
/// @param event 削除したいイベント
/// @return 成否
int
SelectHandler::DeleteTrigger(int fd, int event)
{
  if (fd == DISABLED_FD) {
    return -1;
  }
  if (static_cast<bool>(event & EPOLLIN)) {
    read_fds_.erase(fd);
  }
  if (static_cast<bool>(event & EPOLLOUT)) {
    write_fds_.erase(fd);
  }
  if (static_cast<bool>(event & EPOLLERR)) {
    except_fds_.erase(fd);
  }
  return 0;
}

void
SelectHandler::SetCallback(int fd, int event, callback_t fn)
{
  (void)event;
  // TODO: Selectのコールバックは別で実装する
  reaction_[fd] = fn;
}

/// @brief 指定された条件のコールバックを削除する関数
/// @param fd 削除したいコールバックに関連づくFD
void
SelectHandler::EraseCallback(int fd)
{
  std::erase_if(reaction_, [fd](auto& p) { return p.first == fd; });
  std::cout << "callback erased: " << fd << std::endl;
}

/// @brief イベントを待機する処理
/// @return 準備ができているFD数
int
SelectHandler::WaitEvent()
{
  std::cout << "waiting event by select ..." << std::endl;
  LinkFdSet(read_fds_, read_mask_);
  LinkFdSet(write_fds_, write_mask_);
  LinkFdSet(except_fds_, except_mask_);
  std::cout << "read: " << read_fds_ << std::endl;
  std::cout << "write: " << write_fds_ << std::endl;
  std::cout << "except: " << except_fds_ << std::endl;
  auto fd_width = GetMaxFd() + 1; // +1補正が必要
  auto* to = Timeout();
  auto updated_fd_num =
    select(fd_width, &read_mask_, &write_mask_, &except_mask_, to);
  return updated_fd_num;
}

/// @brief イベント待機のタイムアウト値を取得する
/// @return select用のタイムアウト構造体のアドレス
struct timeval*
SelectHandler::Timeout()
{
  using secs = std::chrono::seconds;
  using usecs = std::chrono::microseconds;
  static struct timeval timeout;
  if (!timeout_) {
    return nullptr;
  }
  auto s = std::chrono::duration_cast<secs>(*timeout_);
  auto us = std::chrono::duration_cast<usecs>(*timeout_ - s);
  timeout.tv_sec = s.count();
  timeout.tv_usec = us.count();
  return &timeout;
}

/// @brief イベント監視ループ関数
/// @param fn イベント検出時に実行するコールバック
void
SelectHandler::RunOnce()
{
  auto updated_fd_num = WaitEvent();
  if (updated_fd_num == -1) {
    perror("select_wait");
    return;
  }
  if (updated_fd_num == 0) {
    std::cout << "timeout" << std::endl;
    return;
  }

  std::set<fd_t> merged_fds;
  merged_fds.insert(read_fds_.begin(), read_fds_.end());
  merged_fds.insert(write_fds_.begin(), write_fds_.end());
  merged_fds.insert(except_fds_.begin(), except_fds_.end());
  std::cout << "fired fds: " << merged_fds << std::endl;
  for (const auto& fd : merged_fds) {
    if (FD_ISSET(fd, &read_mask_)) {
      std::cout << "read event: " << fd << std::endl;
      reaction_[fd](fd);
    }
    if (FD_ISSET(fd, &write_mask_)) {
      std::cout << "write event" << std::endl;
    }
    if (FD_ISSET(fd, &except_mask_)) {
      std::cout << "except event" << std::endl;
    }
  }
  std::cout << std::endl;
}

/// @brief イベント監視ループ関数
/// @param fn イベント検出時に実行するコールバック
void
SelectHandler::Run()
{
  while (true) {
    RunOnce();
  }
}

/// @brief 現在登録されているFD数を計算する関数
/// @return 現在登録されているFD数
int
SelectHandler::GetMaxFd()
{
  auto read_max = read_fds_.empty() ? 0 : *read_fds_.rbegin();
  auto write_max = write_fds_.empty() ? 0 : *write_fds_.rbegin();
  auto except_max = except_fds_.empty() ? 0 : *except_fds_.rbegin();
  return std::max({ read_max, write_max, except_max });
}

void
SelectHandler::LinkFdSet(const std::set<fd_t>& fds, fd_set& mask)
{
  FD_ZERO(&mask);
  for (const auto& fd : fds) {
    FD_SET(fd, &mask);
  }
}
} // namespace event
