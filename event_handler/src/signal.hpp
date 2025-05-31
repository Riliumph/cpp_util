#ifndef INCLUDE_SIGNAL_HPP
#define INCLUDE_SIGNAL_HPP
// STL
#include <iostream>
#include <memory>
// Linux
#include <cstring>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/signal.h>
#include <sys/signalfd.h>
#include <unistd.h>
// event
#include "event/event.hpp"

void
signal_sample(std::shared_ptr<event::interface::EventHandler> e_handler)
{
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);  // Ctrl+C
  sigaddset(&mask, SIGTERM); // Termination signal
  sigprocmask(SIG_BLOCK, &mask, nullptr);
  int sfd = signalfd(-1, &mask, SFD_NONBLOCK | SFD_CLOEXEC);
  e_handler->CreateTrigger(sfd, EPOLLIN);
  e_handler->SetCallback(sfd, EPOLLIN, [sfd](int fd) {
    struct signalfd_siginfo signal;
    auto read_size = read(fd, &signal, sizeof(signal));
    if (read_size != sizeof(signal)) {
      std::cerr << "Error reading signal info" << std::endl;
      return;
    }
    if (signal.ssi_signo == SIGINT) {
      std::cout << "Received SIGINT (Ctrl+C)" << std::endl;
    } else if (signal.ssi_signo == SIGTERM) {
      std::cout << "Received SIGTERM" << std::endl;
    } else {
      std::cout << "Received unknown signal: " << signal.ssi_signo << std::endl;
    }
  });
}
#endif // INCLUDE_SIGNAL_HPP
