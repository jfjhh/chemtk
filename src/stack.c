#include "stack.h"

struct stack *new_stack(void)
{
	struct stack *new;

	if ((new = (struct stack *) malloc(sizeof(struct stack)))) {
		new->data = NULL;
		new->next = NULL;
		new->end = new;
	} else {
		new = NULL;
	}

	return new;
}

void push_stack(struct stack *stack, struct num_str *data)
{
	struct stack *new;

	if (data && (new = (struct stack *) malloc(sizeof(struct stack)))) {
		new->data = data;
		new->next = NULL;
		new->end = new;

		stack->end->next = new;
		stack->end = new;
	} else {
		stack->end->next = NULL;
		stack->end = NULL;
	}
}

struct num_str *pop_stack(struct stack *stack)
{
	struct stack *new_end;
	struct num_str *data = stack->end->data;

	new_end = stack;
	while (new_end->next != stack->end)
		new_end = new_end->next;

	free(stack->end);
	stack->end = new_end;
	stack->end->next = NULL;

	return data;
}

void delete_stack(struct stack *stack)
{
	if (stack) {
		if (stack->next)
			delete_stack(stack->next);

		if (stack->data)
			free(stack->data);
		free(stack);
	}
}

int test_stack(WINDOW *outwin)
{
	int i;
	struct stack *stack;
	const char *test_lines[STACK_TESTS] = {
		"1.23",
		"4.56",
		"7.89",
	};

	if (!(stack = new_stack())) {
		wprintw(outwin, "Could not allocate a test stack (new_stack())!\n");
		return 0;
	}

	for (i = 0; i < STACK_TESTS; i++)
		push_stack(stack, get_num_str(test_lines[i]));

	mvwprintw(outwin, getcury(outwin), 0, "\nDone pushing:\n");
	print_stack(outwin, getcury(outwin), 0, STACK_WIDTH, stack);

	mvwprintw(outwin, getcury(outwin), 0, "\nDone rotating up and down:\n");
	print_stack(outwin, getcury(outwin), 0, STACK_WIDTH, stack);

	delete_stack(stack);
	return 1;
}

void print_stack(WINDOW *outwin, int y, int x, int width, struct stack *stack)
{
	struct stack *iterator;
	int i, fcol, xoff;

	i = 0;
	fcol = 3;
	xoff = x;

	wmove(outwin, y, xoff + ((STACK_WIDTH - 8) / 2)); /* 8 is size of %p string */
	wprintw(outwin, "%p", stack);
	iterator = stack;

	if (stack->next == NULL) {
		mvwhline(outwin, y+(i+1), xoff, 0, width);
		mvwhline(outwin, y+(i+1)+2, xoff, 0, width);
		mvwvline(outwin, y+(i+1), xoff, 0, 3);
		mvwvline(outwin, y+(i+1), xoff+width-1, 0, 3);

		wmove(outwin, getcury(outwin) + 1, xoff + 1);
		for (i = 0; i < STACK_WIDTH - 2; i++)
			wprintw(outwin, "%c", '.');

	} else {
		while ((iterator = iterator->next) != NULL) {
			mvwhline(outwin, y+(i+1), xoff, 0, width);
			mvwhline(outwin, y+(i+1)+2, xoff, 0, width);
			mvwvline(outwin, y+(i+1), xoff, 0, 3);
			mvwvline(outwin, y+(i+1), xoff+width-1, 0, 3);

			wmove(outwin, getcury(outwin) + 1, xoff + 1);
			wprintw(outwin, "%s%s (%d)",
					iterator->data->str,
					((iterator->data->type == CONSTANT) ? " C" : ""),
					sig_figs(iterator->data->str));

			if (getcury(outwin) >= getmaxy(outwin) - 3) {
				xoff += STACK_WIDTH;
				i = 0;
			} else if (getcurx(outwin) + STACK_WIDTH
					>= (getmaxx(outwin) - 2)) {
				i = 0;
				fcol += 3;
				xoff = x + fcol;
			} else {
				i += 2;
			}

		}
	}

	wmove(outwin, getcury(outwin) + 2, 0);
}

int stack_depth(struct stack *stack)
{
	int ok, depth;
	struct stack *iterator;

	depth = 0;
	if (!stack)
		return depth;

	iterator = stack;
	ok = 1;
	while (ok) {
		if (iterator->next)
			iterator = iterator->next;
		else
			ok = 0;
		depth++;
	}

	return depth - 1; /* Includes the final NULL stack, so subtract 1. */
}

struct stack *stack_rotdown_(struct stack *stack)
{
	struct stack *new_beg, *new_end;

	new_beg = stack->next;
	new_end = stack;
	stack->end->next = new_end;
	new_beg->end = new_end;

	return new_beg;
}

struct stack *stack_rotup_(struct stack *stack)
{
	struct stack *old_end, *new_end;

	old_end = stack->end;
	new_end = stack;
	while (new_end->next != stack->end)
		new_end = new_end->next;

	old_end->end = new_end;
	old_end->next = stack;
	new_end->next = NULL;
	new_end->end = new_end;

	return old_end; /* Actually the new_beginning. */
}

