#ifndef INCLUDE_NETWORK_INTERFACE_H
#define INCLUDE_NETWORK_INTERFACE_H
#include <functional>
// Original Lib
#include <event.h>
namespace nw::IF {
class Server
{
public:
  Server() = default;
  ~Server() = default;
  virtual int Establish() = 0;
  virtual void Event(event::IF::EventHandler::callback) = 0;
  virtual bool Start() = 0;
};
}
#endif // INCLUDE_NETWORK_INTERFACE_H
