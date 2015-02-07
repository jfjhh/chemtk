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

