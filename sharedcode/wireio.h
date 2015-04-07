#ifndef WIREIO_H
#define WIREIO_H

struct candlemsg *readcandlemsg(int clntsock);

char *readfield(int clntsock, char *field, int fieldsize);

int makeconnection(char *ip, char *port);

struct candlemsg *sendcandlemsg(struct candlemsg *candlemsg, int sock);

char *sendfield(int sock, char *field, int fieldsize);

struct candlemsg *candleexchange(struct candlemsg *candlemsg, char *ip,
                                 char *port);

#endif
