/**
 * @file chemtk.c
 * @author Alex Striff
 *
 * @brief Main file for program execution.
 */

#include "chemtk.h"

int main(void)
{
	/**
	 * @todo
	 * Parse command line args. They should include:
	 * The log file, which is stderr by default.
	 * Other.
	 */

	printf("ChemTK SciCalc Version %s.\n\n", VERSION);
	printf("Testing ...\n");

	/* Run Tests. */
	if (!run_all_tests(stderr)) {
		printf("Not all tests passed (see logfile)!\n");
		return 1;
	} else {
		printf("All tests passed!\n");
	}

	/* Add all commands to memory. */
	init_sc_commands();
	add_sc_command(fun, CONSTANT_CMD);
	add_sc_command(fun, ELEMENT_CMD);

	page_file(SCICALC_DOCFILE); /* Init/Help file. */

	/* TODO: Start calculating. */
	printf("TODO: Start calculating.\n");

	free_sc_commands();

	return 0;
}

