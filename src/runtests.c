#include "runtests.h"

struct test_array all_tests = {
	{
		test_stack,
		test_scicalc,
		test_pager,
		test_constants,
		test_num_str,
	},
	{
		"stack",
		"scicalc",
		"pager",
		"constants",
		"num_str",
	}
};

static int run_test(WINDOW *outwin, int (*test_function)(WINDOW *outwin),
		const char *test_name)
{
	int status;

	wattron(outwin, A_BOLD);
	wprintw(outwin, "Testing %s ...\n", test_name);
	status = (*test_function)(outwin);
	wattron(outwin, A_BOLD);
	wprintw(outwin, "\nTest %s %s\n", test_name, ((status) ? "Succeeded" : "Failed"));
	wattroff(outwin, A_BOLD);

	return status;
}

int run_tests(WINDOW *outwin, struct test_array tests)
{
	int tests_passed = 0;
	int test;

	for (test = 0; test < NUM_TESTS; test++) {
		tests_passed += run_test(outwin, tests.test_functions[test],
				tests.test_names[test]);

		/* Check if at bottom of window, then clear. */
		if (getcury(outwin) >= getmaxy(outwin)-1)
			werase(outwin);
	}

	werase(outwin);
	wattron(outwin, A_BOLD);
	wprintw(outwin, "Tests Passed: [ %d / %d ].\n", tests_passed, NUM_TESTS);
	wattroff(outwin, A_BOLD);

	return tests_passed;
}

