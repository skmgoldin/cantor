#include "conninfo.h"
#include <stdlib.h>
#include "globalvalues.h"
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

struct conninfo *allocconninfo() {

  struct conninfo *conninfo = malloc(sizeof(struct conninfo));

  conninfo->sock = 0;
  conninfo->ip = malloc(sizeof(char) * IPLEN);
  conninfo->port = malloc(sizeof(char) * PORTLEN);


  return conninfo;
}

struct conninfo *initconninfo(struct conninfo *conninfo,
                              int sock, struct sockaddr *connaddr) {

  struct sockaddr_in locca = *(struct sockaddr_in *) connaddr;
  char *buf = malloc(sizeof(char) * IPLEN);

  conninfo->sock = sock;

  strcpy(conninfo->ip, inet_ntop(AF_INET, &(locca.sin_addr), buf, IPLEN));

 // strcpy(conninfo->ip, inet_ntop(AF_INET, &((struct sockaddr_in *)
  //       connaddr)->sin_addr, buf, IPLEN));

  memset(buf, '0', IPLEN);
  sprintf(buf, "%d", ntohs(((struct sockaddr_in *) connaddr)->sin_port)); 
  strcpy(conninfo->port, buf);

  free(buf);

  return conninfo;
}

int deallocconninfo(struct conninfo *conninfo) {

  close(conninfo->sock);
  free(conninfo->ip);
  free(conninfo->port);
  free(conninfo);

  return 0;
}

