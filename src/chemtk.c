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
	sc_stack *stack;

	printf("ChemTK SciCalc Version %s.\n\n", VERSION);
	printf("Testing ...\n");

	/* Add all commands to memory. */
	init_sc_commands();
	// add_sc_command(fun, CONSTANT_CMD);
	// add_sc_command(fun, ELEMENT_CMD);

	stack = new_sc_stack();

	page_file(SCICALC_DOCFILE); /* Init/Help file. */

	/* TODO: Start calculating. */
	printf("TODO: Start calculating.\n");

	delete_sc_stack(stack, free);
	free_sc_commands();

	return 0;
}

