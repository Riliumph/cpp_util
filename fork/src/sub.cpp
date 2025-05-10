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
  // sleep(30);
  printf("argc: %d\n", argc);
  for (auto i = 0; i < argc; ++i) {
    printf("argv[%d]: %s\n", i, argv[i]);
  }
  sleep(100000);
  return 0;
}
