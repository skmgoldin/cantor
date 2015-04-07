#include "globalvalues.h"
#include <string.h>
#include <stdio.h>

char *validatemsg(char *msg) {

  int msglen = strlen(msg);

  if(*(msg + msglen - 1) == '\n') {
    *(msg + msglen - 1) = '\0';
  }

  return msg;
}
