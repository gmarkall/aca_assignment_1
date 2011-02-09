
CC := gcc
LD := gcc
CFLAGS := -O3 -Wall -Werror
LDFLAGS := -lm

OBJS := main.o subs.o ss_rand.o

.SUFFIXES:	.c .o

%.o:	%.c
	$(CC) $(CFLAGS) $< -c

all: demsettle

demsettle: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o demsettle

clean:
	rm -f *.o demsettle 0 1 rng
