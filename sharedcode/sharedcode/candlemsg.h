#ifndef CANDLEMSG_H
#define CANDLEMSG_H

struct candlemsg {
  char *versionid; 
  char *reqtype;   
  char *stableport;
  char *from;   
  char *to;
  char *msg;     
};

struct candlemsg *alloccandlemsg();

struct candlemsg *packcandlemsg(struct candlemsg *candlemsg, char *reqtype,
                                char *stableport, char *from, char *to,
                                char *msg);

int dealloccandlemsg(struct candlemsg *candlemsg);

#endif
