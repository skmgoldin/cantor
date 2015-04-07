#ifndef SERVERTOOLS_H
#define SERVERTOOLS_H

int makeserver(char *port);

struct sockdata *makesock(char *ip, char *port, struct sockdata *sockdata);

struct addrinfo *gethints(int fam, int socktype, int flags,
                           struct addrinfo *hints);

int getsock(int domain, int type, int protocol);

struct conninfo *getconnection(int sock);

int freeport(int servsock);


#endif
