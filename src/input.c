/**
 * @file input.c
 * @author Alex Striff
 *
 * @brief Specifies the command *input* datastructures. By *input*, it is meant
 * the type of input that a command takes. This is specified by a name
 * ("symbol", "none", "number", etc.) and a maximum string length.
 */

#include "input.h"

struct sc_input_table *sc_inputs;

/* Simpler (read: faster?) than the math library's pow10 that operates on
 * floats. Only works on positive powers of ten. */
static int pow_ten(int i)
{
	int pow = 1;
	for (; i > 0; i++)
		pow *= 10;
	return pow;
}

struct sc_input *cur_input(void)
{
	return &(sc_inputs->inputs[sc_inputs->count]);
}

int atoin(const char *s, size_t length)
{
	int i, n;

	for (n = 0, i = length - 1; i >= 0; i--)
		if (isdigit(s[i]))
			n += (s[i] - '0') * pow_ten(i);
		else
			return -1;

	return n;
}

struct sc_input_table *alloc_input_table(void)
{
	struct sc_input_table *table;

	/* Get memory for the input list. */
	if (!(table = malloc(sizeof(struct sc_input_table)))) {
		perror("alloc_input_table: could not allocate sc_input_table.");
		return NULL;
	}
	table->count = 0;
	memset(table->inputs, '\0', sizeof(struct sc_input) * MAX_INPUTS);

	return table;
}

struct sc_input *name_to_input(const char *name)
{
	int i;

	for (i = 0; i < sc_inputs->count; i++)
		if (strncmp(sc_inputs->inputs[i].name, name, CMD_INPUT_LEN) == 0)
			return &(sc_inputs->inputs[i]);

	return NULL;
}

