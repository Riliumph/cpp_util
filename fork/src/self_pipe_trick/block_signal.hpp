#ifndef SELF_PIPE_TRICK_SIGNAL_BLOCKER_HPP
#define SELF_PIPE_TRICK_SIGNAL_BLOCKER_HPP
// system
#include <signal.h>

namespace self_pipe_trick {

class SignalBlocker
{
public:
  SignalBlocker();
  ~SignalBlocker() = default;

  void lock();
  void unlock();

private:
  sigset_t sigset;
};
} // namespace self_pipe_trick

#endif // SELF_PIPE_TRICK_SIGNAL_BLOCKER_HPP
