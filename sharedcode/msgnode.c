#include "candlemsg.h"
#include "msgnode.h"
#include <stdlib.h>
#include "wireio.h"
#include <string.h>
#include <stdio.h>

int deliverpending(struct usernode *user, struct msglist *list) {

  if(list->head != NULL) {
    list->head = deliverpendinghelper(user, list->head);
  }

  return 0;
}

struct msgnode *deliverpendinghelper(struct usernode *user, struct msgnode *currnode) {

  int i;                                                                      
  for(i = 0; *(currnode->msg->msg + i) != ' '; i++) {;}                           
  char *deliverto = malloc(sizeof(char) * (i + 1));                           
  strncpy(deliverto, currnode->msg->msg, i);                                      
  *(deliverto + i) = '\0';     

  if(strcmp(user->username, deliverto) == 0) {
    dealloccandlemsg(candleexchange(currnode->msg, user->ip, user->port));
    if(currnode->next != NULL) {
      currnode->next = deliverpendinghelper(user, currnode->next);
    }
    dealloccandlemsg(currnode->msg);
    struct msgnode *nextnode = currnode->next;
    free(currnode);
    return nextnode; 
  }

  if(currnode->next != NULL) {
    currnode->next = deliverpendinghelper(user, currnode->next);
  }

  return currnode;
}

int deliverall(struct msgnode *firstmsg, char *ip, char *port) {

  dealloccandlemsg(candleexchange(firstmsg->msg, ip, port));
  dealloccandlemsg(firstmsg->msg);

  if(firstmsg->next == NULL) {
    free(firstmsg);
    return 0;
  }

  deliverall(firstmsg->next, ip, port);
  free(firstmsg);
  return 0;
  
}

struct msgnode *addpendingmsg(struct msglist *list, struct candlemsg *msg) {

  struct msgnode *msgnode = malloc(sizeof(struct msgnode));
  msgnode->msg = msg;
  msgnode->next = NULL;

  if(list->head == NULL) {
    list->head = msgnode;
    return list->head;
  }

  struct msgnode *currnode = list->head;
  while(currnode->next != NULL) {
    currnode = currnode->next;
  }

  currnode->next = msgnode;

  return list->head;
}
