#ifndef TESTS_H
#define TESTS_H

#include <stdio.h>

#include "chemtk.h"

#define NUM_TESTS         8

struct test_array {
	int  (*test_functions[NUM_TESTS])(FILE *logfile);
	char *test_names[NUM_TESTS];
};

struct test_array all_tests;

/* Returns 0 if any tests failed, and the number passed if all passed. */
int run_tests(FILE *logfile, struct test_array tests);
#define run_all_tests(A) run_tests(A, all_tests)

#endif /* TESTS_H */

