
CC := gcc
LD := gcc
CFLAGS := -O3 -Wall -Werror

OBJS := main.o subs.o ss_rand.o

.SUFFIXES:	.c .o

%.o:	%.c
	$(CC) $(CFLAGS) $< -c

all: demsettle

demsettle: $(OBJS)
	$(LD) $(OBJS) -o demsettle

clean:
	rm -f *.o demsettle
