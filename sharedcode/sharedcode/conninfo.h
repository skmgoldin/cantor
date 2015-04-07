#ifndef CONNINFO_H
#define CONNINFO_H

#include <netinet/in.h>

struct conninfo {
  int sock;
  char *ip;
  char *port;
};

struct conninfo *allocconninfo();

struct conninfo *initconninfo(struct conninfo *conninfo,
                              int sock, struct sockaddr *connaddr); 

int deallocconninfo(struct conninfo *conninfo);

#endif
