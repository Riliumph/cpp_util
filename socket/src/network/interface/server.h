#ifndef INCLUDE_NETWORK_INTERFACE_H
#define INCLUDE_NETWORK_INTERFACE_H
namespace nw::IF {
class Server
{
  virtual int Establish() = 0;
};
}
#endif // INCLUDE_NETWORK_INTERFACE_H
