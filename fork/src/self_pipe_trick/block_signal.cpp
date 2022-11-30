// standard
#include <cstddef>
// original
#include "self_pipe_trick/block_signal.hpp"

namespace self_pipe_trick {

SignalBlocker::SignalBlocker()
{
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGCHLD);
}

void
SignalBlocker::lock()
{
  pthread_sigmask(SIG_BLOCK, &sigset, NULL);
}

void
SignalBlocker::unlock()
{
  pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);
}
} // namespace self_pipe_trick
