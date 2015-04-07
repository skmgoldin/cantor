#ifndef CANDLECLIENT_H
#define CANDLECLIENT_H

#include "../sharedcode/userlist.h"

int login(char *servip, char *servport);

int showrunner(char *servip, char *servport, char *mysock, struct userlist *pvtlist);

int inputhandler(char *servip, char *servport, struct userlist *pvtlist);

char **parseinput(char *input);

int logout(char *servip, char *servport);

#endif
