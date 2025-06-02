#ifndef INCLUDE_NETWORK_UTIL_ADDRINFO_H
#define INCLUDE_NETWORK_UTIL_ADDRINFO_H
// STL
#include <optional>
#include <string>
// system
#include <netdb.h>

void
FreeAddrinfo(struct addrinfo*);
int
DeepCopy(const struct addrinfo*, struct addrinfo**);

std::optional<std::string>
ConvertIPv4(struct addrinfo*);

int
ConvertPort(struct addrinfo*);
#endif // INCLUDE_NETWORK_UTIL_ADDRINFO_H
