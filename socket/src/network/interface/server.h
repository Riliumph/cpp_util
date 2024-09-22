#ifndef INCLUDE_NETWORK_INTERFACE_H
#define INCLUDE_NETWORK_INTERFACE_H
#include <functional>
namespace nw::IF {
class Server
{
  virtual int Establish() = 0;
  virtual bool Start(std::function<bool(int)>) = 0;
};
}
#endif // INCLUDE_NETWORK_INTERFACE_H
