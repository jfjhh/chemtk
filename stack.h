#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

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

#endif

