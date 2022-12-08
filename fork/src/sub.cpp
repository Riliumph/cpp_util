#include <cstdio>
// system
#include <fcntl.h>
#include <pthread.h>
#include <sys/select.h>
#include <unistd.h>
// original
#include "self_pipe_trick/block_signal.hpp"

int
main(int argc, char** argv)
{
  printf("argc: %d\n", argc);
  for (auto i = 0; i < argc; ++i) {
    printf("argv[%d]: %s\n", i, *argv[i]);
  }
  return 0;
}
