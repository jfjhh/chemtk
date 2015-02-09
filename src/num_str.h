#ifndef NUM_STR_H
#define NUM_STR_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

#define NUMSTR_BUFSIZE 32
#define NUMSTR_CONST_CHAR '.'

enum num_str_type {
	NONE = 0,
	NUMBER = 1,
	CMD = 2,
	CONSTANT = 3
};

struct num_str {
	long double data;
	enum num_str_type type;
	char str[NUMSTR_BUFSIZE];
};

struct num_str *get_num_str(WINDOW *outwin, const char *prompt);
int test_num_str(WINDOW *outwin);
int get_constant(const char *buf, long double *number);

#endif /* NUM_STR_H */

