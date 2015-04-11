#ifndef NUM_STR_H
#define NUM_STR_H

#include <stdio.h> /* DEBUGGING */
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "stack.h"
#include "constants.h"
#include "operator.h"
#include "ldmath.h"

#define NUMSTR_BUFSIZE 16
#define NUMSTR_CONST_CHAR '.'
#define NUMSTR_PRECISION 4
#define NUMSTR_FIG_TESTS 4

enum num_str_type {
	NONE = 0,
	NUMBER,
	CMD,
	CONSTANT,
	OPERATOR
};

struct num_str {
	long double data;
	int sig_figs;
	enum num_str_type type;
	char str[NUMSTR_BUFSIZE];
};

struct num_str *get_num_str(const char *line);
int test_num_str(WINDOW *outwin);
int get_constant(const char *buf, long double *number);
int sig_figs(const char *line);
int sig_before(const char *line);
int sig_after(const char *line);
int operate(WINDOW *outwin, struct stack *stack);

#endif /* NUM_STR_H */

