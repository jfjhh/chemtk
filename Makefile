CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g -O2 -lm -lreadline
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC));

all: scicalc
	
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

scicalc: $(OBJ)
	$(CC) $(CFLAGS) $^ -o scicalc

clean:
	@rm scicalc *.o 2> /dev/null

.PHONY: all scicalc clean

