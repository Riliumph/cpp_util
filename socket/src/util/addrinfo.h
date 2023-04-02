#include <netdb.h>

void
FreeAddrinfo(struct addrinfo*);
int
DeepCopy(const struct addrinfo*, struct addrinfo**);
