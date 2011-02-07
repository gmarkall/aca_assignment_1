
CC := gcc
LD := gcc
CFLAGS := -O3

OBJS := main.o subs.o

.SUFFIXES:	.c .o

%.o:	%.c
	$(CC) $(CFLAGS) $< -c

all: dem

demsettle: $(OBJS)
	$(LD) $(OBJS) -o demsettle
