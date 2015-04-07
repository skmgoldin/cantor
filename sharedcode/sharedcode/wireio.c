#include <stdlib.h>
#include <sys/socket.h>
#include "wireio.h"
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include "servertools.h"
#include "globalvalues.h"
#include "candlemsg.h"
#include "sockdata.h"
#include <unistd.h>

struct candlemsg *readcandlemsg(int clntsock) {

  char *versionid = malloc(sizeof(char) * VERSIONIDLEN);
  versionid = readfield(clntsock, versionid, VERSIONIDLEN);

  char *reqtype = malloc(sizeof(char) * REQTYPELEN);
  reqtype = readfield(clntsock, reqtype, REQTYPELEN);

  char *stableport = malloc(sizeof(char) * PORTLEN);
  stableport = readfield(clntsock, stableport, PORTLEN);

  char *from = malloc(sizeof(char) * FROMLEN);
  from = readfield(clntsock, from, FROMLEN);

  char *to = malloc(sizeof(char) * FROMLEN);
  to = readfield(clntsock, to, TOLEN);

  char *msg = malloc(sizeof(char) * MSGLEN);
  msg = readfield(clntsock, msg, MSGLEN);

  struct candlemsg *candlemsg = alloccandlemsg();
  candlemsg = packcandlemsg(candlemsg, reqtype, stableport, from, to, msg);

  free(versionid);
  free(reqtype);
  free(stableport);
  free(from);
  free(to);
  free(msg);

  return candlemsg;
}

char *readfield(int clntsock, char *field, int fieldsize) {

  char *buf = malloc(sizeof(char) * fieldsize);
  int len = fieldsize;
  int flags = 0;

  if(recv(clntsock, buf, len, flags) == 0) {
    free(buf);
    buf = NULLFIELD;  
    strcpy(field, buf);
    return field;
  }

  strcpy(field, buf);

  free(buf);

  return field;
}

int makeconnection(char *ip, char *port) {
  struct sockdata *sockdata = allocsockdata();
  sockdata = makesock(ip, port, sockdata);

  if(connect(sockdata->sock, sockdata->servinfo->ai_addr,
             sockdata->servinfo->ai_addrlen) != 0) {
    fprintf(stderr, "%s\n", "connect failed.");
    return 0;
  }

  int sock = sockdata->sock;

  deallocsockdata(sockdata);

  return sock;
}

struct candlemsg *sendcandlemsg(struct candlemsg *candlemsg, int sock) {
  sendfield(sock, candlemsg->versionid, VERSIONIDLEN);
  sendfield(sock, candlemsg->reqtype, REQTYPELEN);
  sendfield(sock, candlemsg->stableport, PORTLEN);
  sendfield(sock, candlemsg->from, FROMLEN);
  sendfield(sock, candlemsg->to, TOLEN);
  sendfield(sock, candlemsg->msg, MSGLEN);

  return candlemsg;
}

char *sendfield(int sock, char *field, int fieldsize) {

  send(sock, field, fieldsize, 0);

  return field;
}

struct candlemsg *candleexchange(struct candlemsg *candlemsg, char *ip,
                                 char *port) {

  int sock = makeconnection(ip, port);
  if(sock == 0) {return NULL;}

  sendcandlemsg(candlemsg, sock);

  struct candlemsg *reply = readcandlemsg(sock);

  close(sock);

  return reply;
}

