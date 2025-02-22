#include "select_handler.h"
// STL
#include <chrono>
#include <iostream>
#include <vector>
// system
#include <sys/epoll.h>
#include <unistd.h>

namespace event {
/// @brief デフォルトコンストラクタ
SelectHandler::SelectHandler()
  : select_fd(0)
  , event_max(EVENT_MAX)
{
}

/// @brief コンストラクタ
/// @param max_event_num 監視イベント最大数
SelectHandler::SelectHandler(size_t max_event_num)
  : select_fd(0)
  , event_max(max_event_num)
{
}

/// @brief デストラクタ
SelectHandler::~SelectHandler()
{
  if (0 < select_fd) {
    close(select_fd);
  }
}

/// @brief 実行可能を判定する関数
/// @return 成否
bool
SelectHandler::CanReady()
{
  return select_fd != -1;
}

/// @brief 監視するイベントを登録する関数
/// @param fd 新たに監視するFD
/// @param event 監視したいイベント
/// @return 成否
int
SelectHandler::CreateTrigger(int fd, int event)
{
  switch (event) {
    case EPOLLIN:
      FD_SET(fd, &read_fds);
      break;
    case EPOLLOUT:
      FD_SET(fd, &write_fds);
      break;
    case EPOLLERR:
      FD_SET(fd, &except_fds);
      break;
    default:
      printf("unknown event: %d\n", event);
      return -1;
  }
  max_fd = GetMaxFd();
  return 0;
}

/// @brief 監視しているイベントを変更する関数
/// @param fd 変更したいFD
/// @param event 変更したいイベント
/// @return 成否
int
SelectHandler::ModifyTrigger(int fd, int event)
{
  (void)fd;
  (void)event;
  return 0;
}

/// @brief 監視しているイベントを削除する関数
/// @param fd 削除したいFD
/// @param event 削除したいイベント
/// @return 成否
int
SelectHandler::DeleteTrigger(int fd, int event)
{
  switch (event) {
    case EPOLLIN:
      FD_CLR(fd, &read_fds);
      break;
    case EPOLLOUT:
      FD_CLR(fd, &write_fds);
      break;
    case EPOLLERR:
      FD_CLR(fd, &except_fds);
      break;
    default:
      printf("unknown event: %d\n", event);
      return -1;
  }
  max_fd = GetMaxFd();
  return 0;
}

void
SelectHandler::SetCallback(int fd, int event, callback fn)
{
  (void)event;
  // TODO: Selectのコールバックは別で実装する
  reaction[fd] = fn;
}

/// @brief イベントを待機する処理
/// @return 準備ができているFD数
int
SelectHandler::WaitEvent()
{
  auto updated_fd_num =
    select(max_fd + 1, &read_fds, &write_fds, &except_fds, &timeout);
  return updated_fd_num;
}

/// @brief イベント待機のタイムアウトを設定する関数
/// @param to タイムアウト
void
SelectHandler::Timeout(std::optional<std::chrono::milliseconds> timeout)
{
  using secs = std::chrono::seconds;
  using usecs = std::chrono::microseconds;
  if (timeout) {
    auto s = std::chrono::duration_cast<secs>(*timeout);
    auto us = std::chrono::duration_cast<usecs>(*timeout - s);
    this->timeout.tv_sec = s.count();
    this->timeout.tv_usec = us.count();
  } else {
    this->timeout.tv_sec = 0;
    this->timeout.tv_usec = 0;
  }
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

  for (int i = 0; i < updated_fd_num; ++i) {
    if (FD_ISSET(i, &read_fds)) {
      auto it = reaction.find(i);
      if (it == reaction.end()) {
        std::cerr << "not found fd(" << i << ")" << std::endl;
        continue;
      }
      it->second(i);
    }
  }
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

/// @brief Epollインスタンスを作成する関数
void
SelectHandler::CreateSelect()
{
  // SELECTにEPOLLのようなインスタンス作成は存在しない
}

/// @brief 現在登録されているFD数を計算する関数
/// @return 現在登録されているFD数
int
SelectHandler::GetMaxFd()
{
  int max = 0;
  for (int i = 0; i < FD_SETSIZE; i++) {
    if (FD_ISSET(i, &read_fds) || FD_ISSET(i, &write_fds) ||
        FD_ISSET(i, &except_fds)) {
      max = std::max(max, i);
    }
  }
  return max;
}
}
