/**
 * @file operator.c
 * @author Alex Striff
 *
 * @brief Checks to see if characters are supported mathematical operators.
 */

#include "operator.h"

const char operators[] = {
	'+', '-', '*', '/',
	'=', '<', '>', '&',
	'^', '~', '%', '|',
	'o', 'l', 'r', 'x',
};

int is_operator(char op)
{
	int i, is_member;
	is_member = 0;

	for (i = 0; i < NUM_OPERATORS; i++)
		if (operators[i] == op)
			is_member = 1;

	return is_member;
}

int test_operator(FILE *logfile)
{
	int i, is_op, status;
	char test_ops[NUM_OPERATOR_TESTS] = {
		'+', '-', 'o', '#', '$',
	};

	/**
	 * @test
	 * Tests if @c NUM_OPERATOR_TESTS example operators are valid operators.
	 */

	for (i = status = 0; i < NUM_OPERATOR_TESTS; i++) {
		is_op = is_operator(test_ops[i]);
		fprintf(logfile, "%c %s an operator.\n",
				test_ops[i], ((is_op) ? "is" : "is not"));
		status += is_op;
	}

	return ((status == 3) ? 1 : 0); /* Only 3 of the tested are actually ops. */
}

