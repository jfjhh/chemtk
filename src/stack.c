/**
 * @file stack.c
 * @author Alex Striff
 *
 * @brief An implementation of a stack.
 */

#include "stack.h"

sc_stack *new_sc_stack(void)
{
	sc_stack *new;

	if ((new = (sc_stack *) malloc(sizeof(sc_stack))))
		new->data = new->next = NULL;

	return new;
}

void push_sc_stack(sc_stack **stack, void *data)
{
	sc_stack *new;

	if ((*stack)->data == NULL) {
		(*stack)->data = data;
	} else {
		if ((new = (sc_stack *) malloc(sizeof(sc_stack)))) {
			new->data = data;
			new->next = *stack;
		}
		*stack = new;
	}
}

void *pop_sc_stack(sc_stack **stack)
{
	sc_stack *new_head;
	void *data;

	if (!(*stack)) {
		free(*stack);
		return NULL;
	} else {
		data     = (*stack)->data;
		new_head = (*stack)->next;
	}

	fprintf(stderr, "\tPop Freeing: %p (sc_stack), new_head: %p\n",
			(void *) *stack, (void *) new_head);
	free(*stack);
	*stack = new_head;

	return data;
}

sc_stack *dup_sc_stack(sc_stack *stack)
{
	sc_stack *new = new_sc_stack();
	sc_stack *tmp = stack;

	if (!new)
		return NULL;

	while (tmp) {
		push_sc_stack(&new, tmp->data);
		tmp = tmp->next;
	}

	return new;
}

void delete_sc_stack(sc_stack *stack, void (*free_data)(void *data))
{
	sc_stack *tmp;
	sc_stack *cur = stack;

	if (stack) {
		while (cur) {
			if (free_data)
				free_data(cur->data);

			tmp = cur->next;
			free(cur);
			cur = tmp;
		}
	}
}

void print_sc_stack(FILE *file, sc_stack *stack, sc_stack_pfun pfun)
{
	sc_stack *iterator;

	fprintf(file, "Printing a stack [%p] (TODO).\n", (void *) stack);

	iterator = stack;
	while (iterator) {
		pfun(iterator->data, file);
		iterator = iterator->next;
	}
	fprintf(file, "[ bottom ]\n");
}

int sc_stack_depth(sc_stack *stack)
{
	int depth;
	sc_stack *iterator;

	depth = 0;
	if (!stack)
		return depth; /* Got a NULL stack. */

	iterator = stack;
	for (depth = 0; iterator != NULL; depth++)
		iterator = iterator->next;

	return depth; /* Includes the final NULL stack, so subtract 1. */
}

void sc_stack_rotdown(sc_stack **stack)
{
	sc_stack *new_beg, *new_end;
	new_beg = new_end = (*stack)->next; /* Sets new top. */

	while (new_end->next != NULL)
		new_end = new_end->next;
	new_end->next = *stack; /* Passed top is now at bottom. */
	(*stack)->next = NULL;

	*stack = new_beg;
}

void sc_stack_rotup(sc_stack **stack)
{
	sc_stack *new_beg, *new_end;

	/* New end is two from NULL (bottom). */
	new_end = *stack;
	while (new_end->next->next != NULL)
		new_end = new_end->next;

	/* Get and place new_beg. */
	new_beg       = new_end->next;
	new_beg->next = *stack; /* now at top. */
	new_end->next = NULL; /* 'capped off' new end. */
	*stack        = new_beg;
}

void sc_print_ptr(void *data, FILE *file)
{
	if (data)
		fprintf(file, "[%p]\n", data);
	else
		fprintf(file, "[NULL]\n");
}

void sc_print_double(void *data, FILE *file)
{
	fprintf(file, "[%f]\n", *(double *) data);
}

int test_sc_stack(FILE *logfile)
{
	int i, status, depth;
	sc_stack *stack, *extra;
	void *data;
	char *test_lines[3] = {
		"1.23",
		"4.56",
		"7.89",
	};

	status = 0;
	stack = extra = NULL;

	/**
	 * @test
	 * Tests if printing an empty (or @c NULL) stack with @c sc_print_ptr()
	 * works.
	 */
	print_sc_stack(logfile, stack, sc_print_ptr);

	/**
	 * @test
	 * Tests if a stack can be allocated.
	 */
	if (!(stack = new_sc_stack())) {
		fprintf(logfile, "Could not allocate a test stack (new_sc_stack())!\n");
		return 0;
	}

	/**
	 * @test
	 * Tests if a stack can be pushed to.
	 */
	for (i = 0; i < 3; i++)
		push_sc_stack(&stack, test_lines[i]);

	fprintf(logfile, "\nDone pushing:\n");
	print_sc_stack(logfile, stack, sc_print_ptr);

	/**
	 * @test
	 * Tests if a stack can be copied.
	 */
	extra = dup_sc_stack(stack);

	/**
	 * @test
	 * Tests if a stack can be rotated down.
	 */
	sc_stack_rotdown(&stack);
	fprintf(logfile, "\nDone rotating down:\n");
	print_sc_stack(logfile, stack, sc_print_ptr);

	/**
	 * @test
	 * Tests if a stack can be rotated up.
	 */
	sc_stack_rotup(&stack);
	fprintf(logfile, "\nDone rotating up:\n");
	print_sc_stack(logfile, stack, sc_print_ptr);

	/**
	 * @test
	 * Tests if a stack's depth can be correctly found.
	 */
	status = ((depth = sc_stack_depth(stack)) == 3) ? 0 : 1;
	fprintf(logfile, "\nDone testing depth: stack is %d deep.\n", depth);

	/**
	 * @test
	 * Tests if a stack can be popped from.
	 */
	fprintf(logfile, "\nPopping from stack.\n");
	for (i = 3; i-- > 0;) {
		data = pop_sc_stack(&stack);
		if (data == test_lines[i])
			status += 1;
		else
			fprintf(stderr, "No popped data!\n");
		print_sc_stack(logfile, stack, sc_print_ptr);
	}
	fprintf(logfile, "Done popping.\n");

	/**
	 * @test
	 * Tests if a stack can be completely removed with @c delete_sc_stack.
	 */
	delete_sc_stack(extra, NULL);

	return ((status == 3) ? 1 : 0); /* 3 is number of successful pops for pass. */
}

