CC = gcc
CFLAGS = -Wall -g -O2 -lm

all: scicalc
	
scicalc: scicalc.c
	$(CC) $(CFLAGS) scicalc.c -o scicalc

clean:
	@rm scicalc *.o 2> /dev/null

.PHONY: all scicalc clean

