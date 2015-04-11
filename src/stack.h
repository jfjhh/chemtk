#ifndef STACK_H
#define STACK_H

#include <ncurses.h>
#include <stdlib.h>

struct stack {
	struct num_str *data;
	struct stack *next;
	struct stack *end;
};

#include "num_str.h"

#define STACK_COLOR_NORMAL 2
#define STACK_WIDTH NUMSTR_BUFSIZE + 5
#define STACK_TESTS 3

/* Allocate memory for a new stack. */
struct stack *new_stack(void);

/* Push to the stack */
void push_stack(struct stack *stack, struct num_str *data);

/* Pop from the stack */
struct num_str *pop_stack(struct stack *stack);

/* Delete everything in a stack */
void delete_stack(struct stack *stack);

/* Test the stack out. */
int test_stack(WINDOW *outwin);

/* Print a stack. */
void print_stack(WINDOW *outwin, int y, int x, int width, struct stack *stack);

/* Find out how deep a stack is */
int stack_depth(struct stack *stack);

#define stack_rotdown(A) (A = stack_rotdown_(A))
struct stack *stack_rotdown_(struct stack *stack);

#define stack_rotup(A) (A = stack_rotup_(A))
struct stack *stack_rotup_(struct stack *stack);

#endif

