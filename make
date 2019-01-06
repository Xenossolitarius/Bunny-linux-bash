CC = gcc
CFLAGS = -Wall

TARGETS = bunny

default: bunny

all: $(TARGETS)

bunny: bunny.o
        $(CC) $(CFLAGS) bunny.o -o bunny

clean:
        rm -f *.o *~ a.out $(TARGETS)

.c.o:
        $(CC) $(CFLAGS) -c $<