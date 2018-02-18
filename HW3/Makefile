INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -lpthread -lrt

HFILES= 
CFILES= pthreadTest.c linkList.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	test 

clean:
	-rm -f *.o *.d
	-rm -f test

test: $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJS) -o $@ $(LIBS)


depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
