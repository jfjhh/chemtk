#include "stack.h"

struct stack *new_stack(void)
{
	struct stack *new;

	if ((new = (struct stack *) malloc(sizeof(struct stack)))) {
		new->data = NULL;
		new->next = NULL;
	} else {
		new       = NULL;
	}

	return new;
}

void push_stack(struct stack **stack, struct num_str *data)
{
	struct stack *new;

	if ((*stack)->data == NULL) {
		(*stack)->data = data;
	} else {
		if ((new = (struct stack *) malloc(sizeof(struct stack)))) {
			new->data = data;
			new->next = *stack;
		}
		*stack = new;
	}
}

struct num_str *pop_stack(struct stack **stack)
{
	struct stack *new_head;
	struct num_str *data;

	if (!(*stack)) {
		return NULL;
	} else {
		data     = (*stack)->data;
		new_head = (*stack)->next;
	}

	fprintf(stderr, "\tPop Freeing: %p (stack), new_head: %p\n",
			(void *) *stack, (void *) new_head);
	free(*stack);
	*stack = new_head;

	return data;
}

void delete_stack(struct stack *stack)
{
	struct stack *cur, *next;

	fprintf(stderr, "Recieved %p (stack) in delete_stack.\n", (void *) stack);

	if ((cur = stack)) {
		fprintf(stderr, "Deleting stack: %p (stack)\n", (void *) cur);
		while (cur != NULL) {
			next = cur->next;
			if (cur->data) {
				fprintf(stderr, "\tDelete Freeing Data: %p (num_str)\n",
						(void *) cur->data);
				free(cur->data);
			} else {
				fprintf(stderr, "\tDid not need to free data (num_str).\n");
			}
			fprintf(stderr, "\tDelete Freeing: %p (stack)\n", (void *) cur);
			free(cur);
			cur = next;
		}
	}
}

int test_stack(WINDOW *outwin)
{
	int i, status, depth;
	struct stack *stack;
	struct num_str *data;
	const char *test_lines[STACK_TESTS] = {
		"1.23",
		"4.56",
		"7.89",
	};
	status = 0;

	fprintf(stderr, "FYI, sizeof(struct num_str) is %ld, "
			"and sizeof(struct stack) is %ld.\n",
			sizeof(struct num_str),
			sizeof(struct stack));

	/* Allocate memory. */
	if (!(stack = new_stack())) {
		wprintw(outwin, "Could not allocate a test stack (new_stack())!\n");
		return 0;
	}

	/* Push. */
	for (i = 0; i < STACK_TESTS; i++)
		push_stack(&stack, get_num_str(test_lines[i]));

	/* Test other ops, like rotations and depth. */
	mvwprintw(outwin,       getcury(outwin),  0,  "\nDone pushing:\n");
	print_stack(outwin,     getcury(outwin),  0,  STACK_WIDTH, stack);

	stack_rotdown(&stack);
	mvwprintw(outwin,       getcury(outwin),  0,  "\nDone rotating down:\n");
	print_stack(outwin,     getcury(outwin),  0,  STACK_WIDTH, stack);

	stack_rotup(&stack);
	mvwprintw(outwin,       getcury(outwin),  0,  "\nDone rotating up:\n");
	print_stack(outwin,     getcury(outwin),  0,  STACK_WIDTH, stack);

	status = ((depth = stack_depth(stack)) == 3) ? 0 : 1;
	mvwprintw(outwin, getcury(outwin), 0,
			"\nDone testing depth: stack is %d deep.\n", depth);

	/* Pop. */
	for (i = STACK_TESTS; i-- > 0;) {
		if ((data = pop_stack(&stack))) {
			if (data->str) {
				status += (strncmp(test_lines[i], data->str, \
							strlen(test_lines[i]))) \
						  ? 1 : 0;
			} else {
				fprintf(stderr, "No popped data->str!\n");
				status += 1;
			}
		} else {
			fprintf(stderr, "No popped data!\n");
			status += 1;
		}

		fprintf(stderr, "Freeing data: %p (num_str).\n", (void *) data);
		free(data);
	}

	/* Alloc again. */
	if (!(stack = new_stack())) {
		wprintw(outwin, "Could not allocate a test stack (new_stack())!\n");
		return 0;
	}

	/* Push again. */
	for (i = 0; i < STACK_TESTS; i++)
		push_stack(&stack, get_num_str(test_lines[i]));

	/* Test delete_stack. */
	fprintf(stderr, "Passing %p (stack) to delete_stack.\n", (void *) stack);
	delete_stack(stack);

	return ((status == 0) ? 1 : 0); /* !0 is failed test. */
}

void print_stack(WINDOW *outwin, int y, int x, int width, struct stack *stack)
{
	struct stack *iterator;
	int i, fcol, xoff;

	i    = 0;
	fcol = 3;
	xoff = x;

	/* 8 is size of %p string */
	wmove(outwin,    y,     xoff + ((STACK_WIDTH - 8) / 2));
	wprintw(outwin,  "%p",  stack);

	iterator = stack;
	while (iterator != NULL) {
		mvwhline(outwin,  y+(i+1),    xoff,          0,  width);
		mvwhline(outwin,  y+(i+1)+2,  xoff,          0,  width);
		mvwvline(outwin,  y+(i+1),    xoff,          0,  3);
		mvwvline(outwin,  y+(i+1),    xoff+width-1,  0,  3);

		wmove(outwin, getcury(outwin) + 1, xoff + 1);

		if (iterator->data) {
			if (iterator->data->type == CONSTANT)
				wprintw(outwin, "%s | C",
						iterator->data->str);
			else
				wprintw(outwin, "%s |%2d",
						iterator->data->str,
						sig_figs(iterator->data->str));
		}

		if (getcury(outwin) >= getmaxy(outwin) - 3) {
			xoff += STACK_WIDTH;
			i    =  0;
		} else if (getcurx(outwin) + STACK_WIDTH
				>= (getmaxx(outwin) - 2)) {
			i    =  0;
			fcol += 3;
			xoff =  x + fcol;
		} else {
			i    += 2;
		}

		iterator = iterator->next;
	}

	wmove(outwin, getcury(outwin) + 2, 0);
}

int stack_depth(struct stack *stack)
{
	int depth;
	struct stack *iterator;

	depth = 0;
	if (!stack)
		return depth; /* Got a NULL stack. */

	iterator = stack;
	for (depth = 0; iterator != NULL; depth++)
		iterator = iterator->next;

	return depth; /* Includes the final NULL stack, so subtract 1. */
}

void stack_rotdown(struct stack **stack)
{
	struct stack *new_beg, *new_end;
	new_beg = new_end = (*stack)->next; /* Sets new top. */

	while (new_end->next != NULL)
		new_end = new_end->next;
	new_end->next = *stack; /* Passed top is now at bottom. */
	(*stack)->next = NULL;

	*stack = new_beg;
}

void stack_rotup(struct stack **stack)
{
	struct stack *new_beg, *new_end;

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

