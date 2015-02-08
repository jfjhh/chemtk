#ifndef STACK_H
#define STACK_H

#include <ncurses.h>
#include <stdlib.h>

#define STACK_COLOR_NORMAL 2

struct stack {
	long double data;
	struct stack *next;
	struct stack *end;
};

/* Allocate memory for a new stack. */
struct stack *new_stack(void);

/* Push to the stack */
void push_stack(struct stack *stack, long double data);

/* Pop from the stack */
long double pop_stack(struct stack *stack);

/* Delete everything in a stack */
void delete_stack(struct stack *stack);

/* Test the stack out. */
int test_stack(WINDOW *outwin);

/* Print a stack. */
void print_stack(WINDOW *outwin, int y, int x, struct stack *stack);

/* Find out how deep a stack is */
int stack_depth(struct stack *stack);

#endif

