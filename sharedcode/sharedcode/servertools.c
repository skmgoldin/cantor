#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "servertools.h"
#include "sockdata.h"
#include "conninfo.h"

#define BACKLOG 10

int makeserver(char *port) {

  struct sockdata *sockdata = allocsockdata();

  sockdata = makesock(NULL, port, sockdata);

  freeport(sockdata->sock);
  if(bind(sockdata->sock, sockdata->servinfo->ai_addr,
          sockdata->servinfo->ai_addrlen) != 0) {
    fprintf(stderr, "%s\n", "Bind failed.");
    exit(1);
  }

  int sock = sockdata->sock;
  deallocsockdata(sockdata);

  return sock;
}

struct sockdata *makesock(char *ip, char *port, struct sockdata *sockdata) {

  sockdata->hints = gethints(AF_UNSPEC, SOCK_STREAM, AI_PASSIVE,
                             sockdata->hints);

  if(getaddrinfo(ip, port, sockdata->hints, &(sockdata->servinfo)) != 0) {
    fprintf(stderr, "%s\n", "getaddrinfo failed.");
    exit(1);
  }

  sockdata->sock = getsock(sockdata->servinfo->ai_family,
                           sockdata->servinfo->ai_socktype,
                           sockdata->servinfo->ai_protocol);

  return sockdata;
}

struct conninfo *getconnection(int sock) {

  int backlog = BACKLOG;
  struct sockaddr_storage clntaddr;
  socklen_t clntaddrsize = sizeof(struct sockaddr_storage);

  struct conninfo *conninfo = allocconninfo();

  listen(sock, backlog);

  int clntsock = accept(sock, (struct sockaddr *) &clntaddr, &clntaddrsize);
  conninfo = initconninfo(conninfo, clntsock, (struct sockaddr *) &clntaddr);
  
  return conninfo;
}

struct addrinfo * gethints(int fam, int socktype, int flags,
                           struct addrinfo *hints) {

  memset(hints, 0, sizeof(struct addrinfo));

  hints->ai_family = fam;
  hints->ai_socktype = socktype;
  hints->ai_flags = flags;

  return hints;
}

int getsock(int domain, int type, int protocol) {
  int sock;

  if((sock = socket(domain, type, protocol)) < 0) {
    fprintf(stderr, "%s\n", "socket() failed.");
    exit(1);
  }

  return sock;
}



int freeport(int servsock) {

  int yes = 1;
  if(setsockopt(servsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) != 0) {
    fprintf(stderr, "%s\n", "Failed to free requested port.");
    exit(1);
  }

  return 1;
}


