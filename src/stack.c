#include "stack.h"

struct stack *new_stack(void)
{
	struct stack *new;

	if ((new = (struct stack *) malloc(sizeof(struct stack)))) {
		new->data = 0;
		new->next = NULL;
		new->end = new;
	} else {
		new = NULL;
	}

	return new;
}

void push_stack(struct stack *stack, long double data)
{
	struct stack *new;

	if ((new = (struct stack *) malloc(sizeof(struct stack)))) {
		new->data = data;
		new->next = NULL;
		new->end = new;

		stack->end->next = new;
		stack->end = new;
	} else {
		new = NULL;
	}
}

long double pop_stack(struct stack *stack)
{
	struct stack *new_end;
	long double data = stack->end->data;

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
	if (stack->next)
		delete_stack(stack->next);
	else
		free(stack);
}

int test_stack(WINDOW *outwin)
{
	int test_status, i;
	struct stack *stack = new_stack();
	const int test_len = 3;
	const long double test_doubles[] = {
		1.23,
		4.56,
		7.89,
	};

	test_status = 1;

	wattroff(outwin, A_BOLD);

	for (i = 0; i < test_len; i++)
		push_stack(stack, test_doubles[i]);

	mvwprintw(outwin, getcury(outwin), 0, "\nDone pushing:\n");
	print_stack(outwin, getcury(outwin), 0, stack);

	mvwprintw(outwin, getcury(outwin), 0, "\nDone rotating up and down:\n");
	print_stack(outwin, getcury(outwin), 0, stack);

	wprintw(outwin, "\nStack depth is %d.\n", stack_depth(stack));

	for (i = test_len - 1; i >= 0; i--) {
		if (pop_stack(stack) != test_doubles[i]) {
			test_status = 0;
			break;
		}
	}

	wprintw(outwin, "\nDone popping:\n");
	print_stack(outwin, getcury(outwin), 0, stack);
	wattron(outwin, A_BOLD);

	delete_stack(stack);
	return test_status;
}

void print_stack(WINDOW *outwin, int y, int x, struct stack *stack)
{
	struct stack *iterator;
	const int width = 16;
	int i = 0;

	wmove(outwin, y + 1, x + 4);
	wprintw(outwin, "%p", stack);
	iterator = stack;

	if (stack->next == NULL) {
		mvwhline(outwin, y+(i+1)+1, x, 0, width);
		mvwhline(outwin, y+(i+1)+3, x, 0, width);
		mvwvline(outwin, y+(i+1)+1, x, 0, 3);
		mvwvline(outwin, y+(i+1)+1, x+width-1, 0, 3);

		wmove(outwin, getcury(outwin) + 1, x + 1);
		wprintw(outwin, ".....EMPTY....");

	} else {
		while ((iterator = iterator->next) != NULL) {
			mvwhline(outwin, y+(i+1)+1, x, 0, width);
			mvwhline(outwin, y+(i+1)+3, x, 0, width);
			mvwvline(outwin, y+(i+1)+1, x, 0, 3);
			mvwvline(outwin, y+(i+1)+1, x+width-1, 0, 3);

			wmove(outwin, getcury(outwin) + 1, x + 1);
			wprintw(outwin, "%0.8Le", iterator->data);

			i += 2;
		}
	}

	wmove(outwin, getcury(outwin) + 2, 0);
}

int stack_depth(struct stack *stack)
{
	int depth;
	struct stack *iterator;
	iterator = stack;
	depth = 1;

	while ((iterator = iterator->next) != NULL)
		depth++;

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

