#ifndef USERLIST_H
#define USERLIST_H

#include <time.h>
#include "candlemsg.h"

struct userlist {
  struct usernode *head;
};

struct usernode {
  char *username;
  char *usid;
  char *ip;
  char *port;
  time_t lastcheckin;
  int util;
  struct msgnode *msg;
  struct usernode *next;
};

struct userlist *adduser(char *username, char *usid, char *ip, char *port,
                         struct userlist *userlist);

struct usernode *initusernode(struct usernode *newnode, char *username,
                              char *usid, char *ip, char *port);

struct userlist *inituserlist(struct userlist *users);

int deinitusernode(struct usernode *node);

struct userlist *rmvusername(char *username, struct userlist *users);

struct userlist *rmvusid(char *usid, struct userlist *userlist);

int deinituserlist(struct userlist *userlist);

int deinitallnodes(struct usernode *currnode);

struct usernode *findusid(char *usid, struct userlist *userlist);

struct usernode *findusername(char *username, struct userlist *userlist);

char *usidtousername(char *usid, struct userlist *userlist);
#endif
