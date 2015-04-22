#ifndef OPERATOR_H
#define OPERATOR_H

#include <ncurses.h>

#define NUM_OPERATORS	16
extern const char operators[NUM_OPERATORS];

#define NUM_OPERATOR_TESTS	5

int is_operator(char op);
int test_operator(WINDOW *outwin);

#endif /* OPERATOR_H */

