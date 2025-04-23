#ifndef INCLUDE_NETWORK_IPV4_ABC_SOCKET_SERVER_H
#define INCLUDE_NETWORK_IPV4_ABC_SOCKET_SERVER_H
// super class
#include "network/interface/server.h"
// STL
#include <memory>
// Original Lib
#include <event.h>

namespace nw {
namespace ipv4 {
using event_handler_t = event::IF::EventHandler;
using event_callback_t = event_handler_t::callback;
namespace abc {
class SocketServer : public nw::IF::Server
{
public: // IF
  virtual int Establish() = 0;
  virtual bool Start() = 0;
  virtual void Event(event_callback_t) = 0;
  virtual void EventHandler(std::shared_ptr<event_handler_t>) = 0;
};
} // namespace abc
} // ipv4
} // nw
#endif // INCLUDE_NETWORK_IPV4_ABC_SOCKET_SERVER_H
