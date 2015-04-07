#ifndef MSGNODE_H
#define MSGNODE_H

#include "userlist.h"

struct msglist {
  struct msgnode *head;
};

struct msgnode {

  struct candlemsg *msg;
  struct msgnode *next;
};

int deliverall(struct msgnode *firstmsg, char *ip, char *port);

struct msgnode *addpendingmsg(struct msglist *list, struct candlemsg *msg);

int deliverpending(struct usernode *user, struct msglist *list);

struct msgnode *deliverpendinghelper(struct usernode *user, struct msgnode *currnode);

#endif
