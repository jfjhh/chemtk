#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

struct stack {
	long double data;
	struct stack *next;
};

/* Allocate memory for a new stack. */
struct stack *new_stack(void);

/* Delete all members down a stack */
void delete_stack(struct stack *stack);

/* Add data to the beginning of the stack, and shift down. */
void begin_stack(struct stack *stack, long double data);

/* Add data to the end of the stack. */
void end_stack(struct stack *stack, long double data);

/* Delete a node with data from the middle of a stack. */
void remove_stack(struct stack *stack, long double data);


#endif

