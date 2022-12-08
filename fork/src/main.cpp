#include <cstdio>
// system
#include <fcntl.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
// original
#include "self_pipe_trick/block_signal.hpp"

int
main()
{
  printf("PID: %d\n", getgid());
  // Start Block
  self_pipe_trick::SignalBlocker sig_block;
  sig_block.lock();

  auto pid = fork();
  if (pid == -1) {
    perror("fork");
    return -1;
  } else if (pid == 0) {
    sleep(10); // Attach用の時間
    // 子プロセス
    // 子プロセスはfdの状態も引き継いでしまう。
    // 子プロセス側の処理でfdをすべて閉じる
    // static const int start_fd = 3;
    // for (auto i = start_fd; i < FD_SETSIZE; ++i) {
    //   close(i);
    // }
    auto argc = 0;
    char* argv[1024];
    argv[argc++] = "fork_sub";
    argv[argc++] = "foo";
    argv[argc++] = "bar";
    argv[argc++] = NULL;
    execve(argv[0], argv, NULL);
    // 以降、到達の場合はエラー
    _exit(-1);
  }
  // 親プロセス
  sig_block.unlock();
  // End Block

  // 子プロセスの終了待機
  int status;
  wait(&status);
  /* 終了ステータスのチェック */
  if (WIFEXITED(status)) {
    printf("親プロセス : 子プロセスは終了ステータス%dで正常終了しました\n",
           WEXITSTATUS(status));
  }
  if (WIFSIGNALED(status)) {
    printf("親プロセス : 子プロセスはシグナル番号%dで終了しました\n",
           WTERMSIG(status));
  }
  return 0;
}
