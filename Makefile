
CC := gcc
LD := gcc
CFLAGS := -O3 -Wall -Werror
LDFLAGS := -lm

OBJS := main.o subs.o ss_rand.o

DEBUG_OBJS := main.o subs.o dummy_rand.o

.SUFFIXES:	.c .o

%.o:	%.c
	$(CC) $(CFLAGS) $< -c

all: demsettle

demsettle: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o demsettle

debug: $(DEBUG_OBJS)
	$(LD) $(LDFLAGS) $(DEBUG_OBJS) -o demsettle

rng: rng.o ss_rand.o
	$(LD) $(LDFLAGS) $^ -o rng

clean:
	rm -f *.o demsettle 0 1 rng
