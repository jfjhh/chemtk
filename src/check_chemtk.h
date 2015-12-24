/**
 * @file check_chemtk.h
 * @author Alex Striff
 *
 * @brief Basic system for running tests.
 */

#ifndef CHECK_CHEMTK_H
#define CHECK_CHEMTK_H

#include <stdio.h>

#include "chemtk.h"

/**
 * Number of tests to be run.
 */
#define NUM_TESTS	10

/**
 * A record structure to store arrays of all tests to run.
 */
struct test_array {
	int  (*test_functions[NUM_TESTS])(FILE *logfile); /**< Function pointers. */
	char *test_names[NUM_TESTS];                      /**< Test names. */
};

/**
 * Stores all test functions and names that are ran by @c run_tests().
 */
struct test_array all_tests;

/**
 * Runs the tests for a single test array.
 *
 * @param logfile The file to log test output to.
 * @param tests The test to run.
 */
int run_tests(FILE *logfile, struct test_array tests);

/**
 * @def run_all_tests(A)
 * @brief Runs all project tests (found in @c all_tests).
 *
 * @param A The file to log test output to.
 */
#define run_all_tests(A) run_tests(A, all_tests)

#endif /* CHECK_CHEMTK_H */

