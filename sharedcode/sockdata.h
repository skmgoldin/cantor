#ifndef SOCKDATA_H
#define SOCKDATA_H

struct sockdata {
  int sock;
  struct addrinfo *servinfo;
  struct addrinfo *hints;
};

struct sockdata *allocsockdata();

int deallocsockdata(struct sockdata *sockdata);

#endif
