#include "candlemsg.h"
#include "globalvalues.h"
#include <stdlib.h>
#include <string.h>
#include "msgvalidation.h"

struct candlemsg *alloccandlemsg() {

  struct candlemsg *candlemsg = malloc(sizeof(struct candlemsg));

  candlemsg->versionid = malloc(sizeof(char) * VERSIONIDLEN);
  memset(candlemsg->versionid, 0, VERSIONIDLEN);

  candlemsg->reqtype = malloc(sizeof(char) * REQTYPELEN);
  memset(candlemsg->reqtype, 0, REQTYPELEN);

  candlemsg->stableport = malloc(sizeof(char) * PORTLEN);
  memset(candlemsg->stableport, 0, PORTLEN);

  candlemsg->from = malloc(sizeof(char) * FROMLEN);
  memset(candlemsg->from, 0, FROMLEN);

  candlemsg->to = malloc(sizeof(char) * TOLEN);
  memset(candlemsg->to, 0, TOLEN);

  candlemsg->msg = malloc(sizeof(char) * MSGLEN);
  memset(candlemsg->msg, 0, MSGLEN);

  return candlemsg;
}

struct candlemsg *packcandlemsg(struct candlemsg *candlemsg, char *reqtype,
                                char * stableport, char *from, char *to,
                                char *msg) {

  strcpy(candlemsg->versionid, VERSIONID);

  reqtype = validatemsg(reqtype);
  strcpy(candlemsg->reqtype, reqtype);

  stableport = validatemsg(stableport);
  strcpy(candlemsg->stableport, stableport);

  from = validatemsg(from);
  strcpy(candlemsg->from, from);

  to = validatemsg(to);
  strcpy(candlemsg->to, to);

  msg = validatemsg(msg);
  strcpy(candlemsg->msg, msg);

  return candlemsg;
}

int dealloccandlemsg(struct candlemsg *candlemsg) {

  free(candlemsg->versionid);
  free(candlemsg->reqtype);
  free(candlemsg->stableport);
  free(candlemsg->from);
  free(candlemsg->to);
  free(candlemsg->msg);

  free(candlemsg);

  return 0;
}
