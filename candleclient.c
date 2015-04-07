#include <stdio.h>
#include <stdlib.h>
#include "../sharedcode/wireio.h"
#include "candleclient.h"
#include "../sharedcode/globalvalues.h"
#include <string.h>
#include "../sharedcode/candlemsg.h"
#include "../sharedcode/servertools.h"
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include "../sharedcode/sockdata.h"
#include <sys/socket.h>
#include <netdb.h>
#include <sys/wait.h>
#include "../sharedcode/userlist.h"

static char *servip;
static char *servport;
static char *username;
static char *listenport;
static char *usid;
static pid_t pid;
  
void siginthandler() {
  logout(servip, servport);
  free(username);
  exit(1);
}

int logout(char *servip, char *servport) {

  struct candlemsg *candlemsg = alloccandlemsg();
  candlemsg = packcandlemsg(candlemsg, LOGOUT, servport, usid, NULLFIELD, NULLFIELD);
  struct candlemsg *reply = candleexchange(candlemsg, servip, servport);
  dealloccandlemsg(candlemsg);
  dealloccandlemsg(reply);
  kill(pid, SIGINT);
  return 0;
}

int main(int argc, char **argv) {

  signal(SIGINT, siginthandler);

  if(*(argv + 1) == NULL || *(argv + 2) == NULL) {
    printf("%s\n", "Run again with an ip address and port number to connect "
           "to.");
    exit(1);
  }

  servip = *(argv + 1);
  servport = *(argv + 2);

  struct userlist *pvtlist = malloc(sizeof(struct userlist));
  pvtlist = inituserlist(pvtlist);

  /* Get a socket to any open port. */
  int servsock = makeserver("0");

  /* Get info about our socket, like the port number. */
  struct sockaddr *servinfo = malloc(sizeof(struct sockaddr));
  socklen_t addrlen = sizeof(struct sockaddr);
  memset(servinfo, '0', sizeof(struct sockaddr));
  getsockname(servsock, servinfo, &addrlen);

  char *myport = malloc(sizeof(char) * IPLEN);   
  sprintf(myport, "%d", ntohs(((struct sockaddr_in *) servinfo)->sin_port)); 
  listenport = malloc(sizeof(char) * PORTLEN);
  strcpy(listenport, myport);
  printf("%s\n", listenport);
  free(myport);

  char *mysock = malloc(sizeof(char) * IPLEN);   
  sprintf(mysock, "%d", servsock);
  free(servinfo);

  /* Main loop */
  showrunner(servip, servport, mysock, pvtlist); 
  
  return 0;
}

int showrunner(char *servip, char *servport, char *mysock, struct userlist *pvtlist) {

  pid = fork();
  int status;

  if(pid == 0) {
    /* Child process */
    execl("./clientlistener", "./clientlistener", mysock, NULL);
  } else if(pid > 0) {
    /* Parent process */
    login(servip, servport);
    while(1) {
      if(waitpid(pid, &status , WNOHANG)) {
        exit(0);
      }
      inputhandler(servip, servport, pvtlist);
    }
  }

  return 0;
}

int inputhandler(char *servip, char *servport, struct userlist *pvtlist) {

  char *input = malloc(sizeof(char) * MSGLEN);

  fgets(input, MSGLEN, stdin);

  /* Tokenize first word */
  int i;
  for(i = 0; *(input + i) != ' ' && *(input + i) != '\n'; i++) {;}
  char *reqtype = malloc(sizeof(char) * (i + 1));
  strncpy(reqtype, input, i);
  *(reqtype + i) = '\0';

  int k = i + 1; // K is the begining of the second word in the input

  if(strcmp(reqtype, LOGOUT) == 0) {
    logout(servip, servport);
    free(reqtype);
    free(input);
    exit(0);
  }

  if(strcmp(reqtype, PRIVATE) == 0) {
    /* Tokenize username from input */
    for(i = k; *(input + i) != ' ' && *(input + i) != '\n'; i++) {;}
    char *pvtuser = malloc(sizeof(char) * (FROMLEN));
    strncpy(pvtuser, input + k, i - k);
    *(pvtuser + i - k) = '\0';

    struct usernode *user = findusername(pvtuser, pvtlist);
    if(user == NULL) {
      printf("%s\n", "You don't have that user's contact information.");
      return 0;
    }

    struct candlemsg *candlemsg = alloccandlemsg();
    candlemsg = packcandlemsg(candlemsg, reqtype, listenport, username, NULLFIELD, (input + k));
   
    struct candlemsg *reply = candleexchange(candlemsg, user->ip, user->port);
    if(reply == NULL) {
      printf("%s\n", "That user is not online. Try the 'message' command to send a message they'll receive when they come back online. It will be stored on the server though! Watch out for NSA goons!");
      dealloccandlemsg(candlemsg);
      free(reqtype);
      free(pvtuser);
      free(input);
      return 0;
    }
   
    dealloccandlemsg(reply);
    dealloccandlemsg(candlemsg);
    free(reqtype);
    free(pvtuser);
    free(input);
    return 0;
  }

  /* Pack and send message to server. */
  struct candlemsg *candlemsg = alloccandlemsg();
  candlemsg = packcandlemsg(candlemsg, reqtype, listenport, usid, NULLFIELD, (input + k));
  struct candlemsg *reply = candleexchange(candlemsg, servip, servport);

  if(strcmp(reply->reqtype, REQFAIL) == 0) {
    /* Something went wrong. Print the server's response, if any. */
    printf("%s\n", reply->msg);

    dealloccandlemsg(candlemsg);
    dealloccandlemsg(reply);
    free(reqtype);
    free(input);

    return 0;
  }

  if(strcmp(reply->reqtype, GETADDRESS) == 0) {
    /* Response to a getaddress request, add the user to the pvtlist. */
    adduser(reply->to, NULLFIELD, reply->msg, reply->stableport, pvtlist);
  }

  dealloccandlemsg(candlemsg);
  dealloccandlemsg(reply);
  free(reqtype);
  free(input);
  return 0;
}

int login(char *servip, char *servport) {

  char *usernamebuf = malloc(sizeof(char) * FROMLEN);
  char *msg = malloc(sizeof(char) * MSGLEN);

  /* Get user input. */
  printf("%s", "Username: ");
  fgets(usernamebuf, FROMLEN, stdin);
  printf("%s", "Password: ");
  fgets(msg, MSGLEN, stdin);

  /* Pack and send login request to server */
  struct candlemsg *candlemsg = alloccandlemsg();
  candlemsg = packcandlemsg(candlemsg, LOGIN, listenport, usernamebuf, NULLFIELD, msg);
  free(msg);
  struct candlemsg *reply = candleexchange(candlemsg, servip, servport);
  dealloccandlemsg(candlemsg);

  if(strcmp(reply->reqtype, NEWUSID) == 0) {
    /* Login was successful, USID assigned. */
    usid = malloc(sizeof(char) * USIDLEN);
    strcpy(usid, reply->msg);

    username = malloc(sizeof(char) * FROMLEN);
    strcpy(username, usernamebuf);
    free(usernamebuf);

    dealloccandlemsg(reply);

  } else if(strcmp(reply->reqtype, AUTHFAIL) == 0) {
    /* Login unsuccessful. Print server instructions, if any. */
    printf("%s\n", reply->msg);
    dealloccandlemsg(reply);
    free(usernamebuf);
    return login(servip, servport);
  }

  return 0;
}
