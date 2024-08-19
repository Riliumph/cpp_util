#include "socket.h"
// STL
#include <stdio.h>
// Linux
#include <sys/socket.h>

/// @brief TIME_WAIT状態のポートを再利用する設定を有効化する。
/// socketに対する設定のため、bind前に実行する必要がある。
/// @return 成功可否
int
ReuseAddress(int fd)
{
  int on = 1;
  auto ok = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  if (ok < 0) {
    perror("setsockopt(,,SO_REUSEADDR)");
    return ok;
  }
  return ok;
}
