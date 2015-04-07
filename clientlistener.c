#include <stdio.h>
#include "sharedcode/servertools.h"
#include "sharedcode/wireio.h"
#include "sharedcode/conninfo.h"
#include "sharedcode/candlemsg.h"
#include <stdlib.h>
#include "sharedcode/globalvalues.h"
#include <string.h>

int main(int argc, char **argv) {

  int servsock = atoi(*(argv + 1));

  while(1) {
    struct conninfo *conninfo = getconnection(servsock);

    struct candlemsg *candlemsg = readcandlemsg(conninfo->sock);

    if(strcmp(candlemsg->reqtype, REQFAIL) == 0) {
      printf("%s\n", candlemsg->msg);
    }

    if(strcmp(candlemsg->reqtype, ONLINE) == 0) {
      printf("%s%s\n", candlemsg->msg, " is online.");
    }

    if(strcmp(candlemsg->reqtype, PRIVATE) == 0) {
      int i;
      for(i = 0; *(candlemsg->msg + i) != ' ' && *(candlemsg->msg + i) != '\n'; i++) {;}

      printf("%s%s%s%s\n", "P2P message from ", candlemsg->from, ": ", candlemsg->msg + i + 1);
    }

    if(strcmp(candlemsg->reqtype, PING) == 0) {
      printf("%s\n", "Checking in with server...");
      struct candlemsg *rply = alloccandlemsg();
      rply = packcandlemsg(rply, PING, NULLFIELD, NULLFIELD, NULLFIELD, NULLFIELD);
      sendcandlemsg(rply, conninfo->sock);
      dealloccandlemsg(rply);
    }

    if(strcmp(candlemsg->reqtype, BROADCAST) == 0) {
      printf("%s\n", candlemsg->msg);
    }

    if(strcmp(candlemsg->reqtype, MESSAGE) == 0) {

      int i;                                                                      
      for(i = 0; *(candlemsg->msg + i) != ' '; i++) {;}                           
      char *to = malloc(sizeof(char) * (i + 1));                           
      strncpy(to, candlemsg->msg, i);                                      
      *(to + i) = '\0';

      printf("%s%s%s%s%s\n", candlemsg->from, " (to ", to, "): ", candlemsg->msg + i + 1);
    }

    deallocconninfo(conninfo); // Connection closed here
    dealloccandlemsg(candlemsg);
  }

  return 0;

}
