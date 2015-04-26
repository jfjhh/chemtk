#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdio.h>

typedef struct sc_stack {
	void            *data;
	struct sc_stack *next;
} sc_stack;

/* Allocate memory for a new stack. */
sc_stack *sc_new_stack(void);

/* Push to the stack */
void sc_push_stack(sc_stack **stack, void *data);

/* Pop from the stack */
void *sc_pop_stack(sc_stack **stack);

/* Print a stack. */
void sc_print_stack(FILE *file, sc_stack *stack,
		void (*print_function)(void *data, FILE *file));

/* Find out how deep a stack is */
int sc_stack_depth(sc_stack *stack);

/* "Rotate" a stack up or down (first element to bottom and vice versa). */
void sc_stack_rotdown(sc_stack **stack);
void sc_stack_rotup(sc_stack **stack);

/* Test the stack out. */
int test_sc_stack(FILE *logfile);

#endif

