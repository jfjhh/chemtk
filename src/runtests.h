#ifndef RUNTESTS_H
#define RUNTESTS_H

#include <ncurses.h>

#include "chemtk.h"

#define NUM_TESTS 7

struct test_array {
	int (*test_functions[NUM_TESTS])(WINDOW *outwin);
	char *test_names[NUM_TESTS];
};

struct test_array all_tests;

int run_tests(WINDOW *outwin, struct test_array tests);
#define run_all_tests(A) run_tests(A, all_tests)

#endif /* RUNTESTS_H */

