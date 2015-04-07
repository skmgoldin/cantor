CC = gcc
LD = gcc
CFLAGS = -g -Wall
LDFLAGS =
CLNTSRCS = candleclient.c $(SHAREDSRCS)
CLNTLSTNSRCS = clientlistener.c $(SHAREDSRCS)
SHAREDSRCS = wireio.c servertools.c msgvalidation.c candlemsg.c userlist.c conninfo.c sockdata.c msgnode.c
CLNTOBJS = $(CLNTSRCS:.c=.o)
CLNTLSTNOBJS = $(CLNTLSTNSRCS:.c=.o)
SHARED = sharedcode/
CLNT = clientcode/

CLNTPROG = client
CLNTLSTNPROG = clientlistener

all: clean $(CLNTPROG) $(CLNTLSTNPROG)

$(CLNTPROG): $(CLNTOBJS)
	$(LD) $(LDFLAGS) $(CLNTOBJS) -o $(CLNTPROG)

$(CLNTLSTNPROG): $(CLNTLSTNOBJS)
	$(LD) $(LDFLAGS) $(CLNTLSTNOBJS) -o $(CLNTLSTNPROG)

%.o: $(SHARED)%.c
	$(CC) $(CFLAGS) -c $<

%.o: $(CLNT)%.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o $(CLNTPROG) $(CLNTLSTNPROG) 
