#include "chemtk.h"

int main(void)
{
	/*
	 * TODO: Parse command line args. They should include:
	 * The log file, which is stderr by default.
	 * Other.
	 * */

	printf("ChemTK SciCalc Version %s.\n\n", VERSION);
	printf("Testing ...\n");

	/* Run Tests. */
	if (!run_all_tests(stderr)) {
		printf("Not all tests passed (see logfile)!\n");
		return 1;
	} else {
		printf("All tests passed!\n");
	}

	page_file(SCICALC_DOCFILE); /* Init/Help file. */

	/* TODO: Start calculating. */
	printf("TODO: Start calculating.\n");

	return 0;
}

