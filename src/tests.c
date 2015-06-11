/**
 * @file tests.c
 * @author Alex Striff
 *
 * @brief Basic system for running tests.
 */

#include "tests.h"

struct test_array all_tests = {
	{
		test_sc_stack,
		test_scicalc,
		test_pager,
		test_constant,
		test_sc_token,
		test_directory,
		test_element,
		test_operator,
	},
	{
		"sc_stack",
		"scicalc",
		"pager",
		"constant",
		"sc_token",
		"directory",
		"element",
		"operator",
	}
};

#define fprintline(A, B)	fprintlinel(A, B, 79)
static void fprintlinel(FILE *file, char c, int l)
{
	int i;

	for (i = 0; i < l; i++)
		fputc(c, file);
	fputc ('\n', file);
}

static int run_test(FILE *logfile, int (*test_function)(FILE *logfile),
		const char *test_name)
{
	int status;

	fprintline(logfile, '=');
	fprintf(logfile, "Testing %s ...\n", test_name);
	status = (*test_function)(logfile);
	fprintf(logfile, "\nTest %s %s\n", test_name,
			((status) ? "Succeeded" : "Failed"));

	return status;
}

int run_tests(FILE *logfile, struct test_array tests)
{
	int tests_passed = 0;
	int test, test_stats[NUM_TESTS];

	fprintline(logfile, '*');
	fprintf(logfile, "\tTESTING (%d tests)\tDATA_DIR is '%s'.\n",
			NUM_TESTS, DATA_DIR);
	fprintline(logfile, '*');
	fputc('\n', logfile);

	/* Run the tests. */
	for (test = 0; test < NUM_TESTS; test++) {
		test_stats[test] =  run_test(logfile, tests.test_functions[test],
				tests.test_names[test]);
		tests_passed     += test_stats[test];
	}

	/* Print results. */
	fprintline(logfile, '*');
	fprintf(logfile, "Results:\n");
	fputc('\t', logfile);
	fprintlinel(logfile, '-', 36); /* 36 is width of table. */
	for (test = 0; test < NUM_TESTS; test++)
		fprintf(logfile, "\t| test_%-13s| %-13s|\n",
				tests.test_names[test],
				(test_stats[test]) ? "Succeeded" : "Failed");
	fputc('\t', logfile);
	fprintlinel(logfile, '-', 36);

	if (tests_passed < NUM_TESTS)
		fprintf(logfile, "\n\tOnly %d of %d tests passed (%.1f%%).\n\n",
				tests_passed, NUM_TESTS,
				((float) tests_passed / (float) NUM_TESTS) * 100.0);
	else
		fprintf(logfile, "\n\tAll %d tests passed!\n\n", NUM_TESTS);

	fprintline(logfile, '*');

	return ((tests_passed == NUM_TESTS) ? NUM_TESTS : 0);
}

