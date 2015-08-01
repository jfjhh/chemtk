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
	sc_stack *stack = NULL;

	printf("ChemTK SciCalc Version %s.\n\n", VERSION);
	printf("Testing ...\n");

	/* Add all commands to memory. */
	if (!init_sc_commands())
		goto exit;

	if (!(stack = new_sc_stack()))
		goto exit;

	page_file(SCICALC_DOCFILE); /* Init/Help file. */

	/* TODO: Start calculating. */
	printf("TODO: Start calculating.\n");

exit:
	delete_sc_stack(stack, free);
	free_sc_commands();

	return 0;
}

