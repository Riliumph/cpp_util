#ifndef INCLUDE_TCP_UTIL_ADDRINFO_H
#define INCLUDE_TCP_UTIL_ADDRINFO_H
// STL
#include <optional>
#include <string>
// C
#include <netdb.h>

void
FreeAddrinfo(struct addrinfo*);
int
DeepCopy(const struct addrinfo*, struct addrinfo**);

std::optional<std::string>
ConvertIPv4(struct addrinfo*);

int
ConvertPort(struct addrinfo*);
#endif // INCLUDE_TCP_UTIL_ADDRINFO_H
