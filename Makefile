CC = gcc
LIBS = -lm -lncurses
CFLAGS = -Wall -Wextra -pedantic -g -O2 $(LIBS)

PREFIX ?= /usr/local

SRC = $(wildcard src/*.c)
OBJ = $(patsubst %.c,%.o,$(SRC))
NAME = chemtk

STACK_LIBNAME = stack
STACK_SRC = src/$(STACK_LIBNAME).c
STACK_OBJ = src/$(STACK_LIBNAME).o
STACK_LIB = build/lib$(STACK_LIBNAME).so

all: build $(NAME) $(STACK_LIB)

# $(OBJ): $(SRC)
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o bin/$@

$(STACK_LIB): $(STACK_SRC)
	$(CC) $(CFLAGS) -fPIC -shared $^ -o $@

clean:
	rm bin/$(NAME) $(STACK_LIB) $(OBJ) 2> /dev/null

build:
	mkdir -p bin
	mkdir -p build

install: install-lib install-bin

install-lib: $(STACK_LIB)
	install $(STACK_LIB) $(PREFIX)/lib/

install-bin: $(NAME)
	install $(NAME) $(PREFIX)/bin/

.PHONY: all scicalc clean setup install install-lib install-bin

