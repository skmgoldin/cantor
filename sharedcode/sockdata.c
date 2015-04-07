#include <stdlib.h>
#include "sockdata.h"
#include <netdb.h>

struct sockdata *allocsockdata() {

  struct sockdata *sockdata = malloc(sizeof(struct sockdata));

  sockdata->servinfo = NULL;
  sockdata->hints = malloc(sizeof(struct addrinfo));

  return sockdata;
}

int deallocsockdata(struct sockdata *sockdata) {
  freeaddrinfo(sockdata->servinfo);
  freeaddrinfo(sockdata->hints);
  free(sockdata);

  return 0;
}
