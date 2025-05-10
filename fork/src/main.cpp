#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string>
// system
#include <fcntl.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
// original
#include "self_pipe_trick/block_signal.hpp"

char** environ;

int
main(int argc, char** argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <arg1> <arg2>\n", argv[0]);
    return 1;
  }
  printf("PID: %d\n", getgid());
  for (int i = 0; i < 3; ++i) {
    auto pid = fork();
    if (pid == -1) {
      perror("fork");
      return -1;
    }
    if (pid == 0) {
      printf("%d child process: %d\n", i, getpid());
      // 子プロセス
      // 子プロセスはfdの状態も引き継いでしまう。
      // 子プロセス側の処理でfdをすべて閉じる
      static constexpr int start_fd = 3;
      for (auto fd = start_fd; fd < FD_SETSIZE; ++fd) {
        close(fd);
      }
      static constexpr int sleep_time = 1;
      printf("sleep %d sec\n", sleep_time);
      sleep(sleep_time); // Attach用の時間
      int exec_argc = 0;
      char* exec_argv[1024];
      for (int i = 1; i < argc; ++i) {
        exec_argv[exec_argc++] = argv[i];
      }
      exec_argv[argc++] = NULL; // NULL終端
      execve(exec_argv[0], exec_argv, environ);
      // 以降、到達の場合はエラー
      printf("error: %s", strerror(errno));
      _exit(-1);
    } else {
      // 子プロセスの終了待機
      int status = 0;
      waitpid(pid, &status, 0);
      /* 終了ステータスのチェック */
      if (WIFEXITED(status)) {
        printf("親プロセス : 子プロセスは終了ステータス%dで正常終了しました\n",
               WEXITSTATUS(status));
      }
      if (WIFSIGNALED(status)) {
        printf("親プロセス : 子プロセスはシグナル番号%dで終了しました\n",
               WTERMSIG(status));
      }
    }
  }
  return 0;
}
