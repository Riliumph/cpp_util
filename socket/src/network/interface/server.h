#ifndef INCLUDE_NETWORK_INTERFACE_SERVER_H
#define INCLUDE_NETWORK_INTERFACE_SERVER_H
// STL
#include <functional>

namespace nw {
namespace interface {
class Server
{
public:
  virtual int Establish() = 0;
  virtual bool Start() = 0;
};
} // namespace interface
} // namespace nw
#endif // INCLUDE_NETWORK_INTERFACE_SERVER_H
