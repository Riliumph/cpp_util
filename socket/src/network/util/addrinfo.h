#ifndef INCLUDE_TCP_UTIL_ADDRINFO_H
#define INCLUDE_TCP_UTIL_ADDRINFO_H
#include <netdb.h>

void
FreeAddrinfo(struct addrinfo*);
int
DeepCopy(const struct addrinfo*, struct addrinfo**);

#endif // INCLUDE_TCP_UTIL_ADDRINFO_H
