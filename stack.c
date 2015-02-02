#include "stack.h"

struct stack *new_stack(void)
{
	struct stack *new;

	if ((new = (struct stack *) malloc(sizeof(struct stack)))) {
		new->data = 0;
		new->next = NULL;
	} else {
		new = NULL;
	}

	return new;
}

void delete_stack(struct stack *stack)
{
	if (stack->next)
		delete_stack(stack->next);

	free(stack);
}

void begin_stack(struct stack *stack, long double data)
{
	long double old_data, cur_data;
	struct stack *cur, *end;

	end = new_stack();

	cur = stack;
	old_data = cur->data;
	cur->data = data;

	while (cur->next) {
		cur = cur->next;
		cur_data = cur->data;
		cur->data = old_data;
		old_data = cur_data;
	}

	end->data = old_data;
}

void end_stack(struct stack *stack, long double data)
{
	struct stack *cur, *new;

	new = new_stack();
	new->data = data;

	cur = stack;
	while (cur->next)
		cur = cur->next;

	cur->next = new;
}

void remove_stack(struct stack *stack, long double data)
{
	struct stack *cur, *new;

	cur = stack;
	while (cur->next && cur->data != data)
		cur = cur->next;

	if (!cur->next) {
		end_stack(stack, data);
	} else {
		new = new_stack();
		new->data = data;
	}
}

