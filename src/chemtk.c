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

	puts("\033[0;1m"); // start bold.
	puts("ChemTK SciCalc Version " VERSION ".\n\n");

	puts("[ Parsing Command File... ]\n");
	puts("\033[0m"); // end bold.
	if (parse_command_file(COMMAND_FILE, stderr) != 0) { // Init. commands.
		fputs("Failed to parse command file (init sc_commands)!\n", stderr);
		goto exit;
	} else if (!(stack = new_sc_stack())) { // Init. stack.
		fputs("Failed to init sc_stack!\n", stderr);
		free_command_info();
		goto exit;
	}

	puts("\033[0;1m"); // start bold.
	print_sc_commands(stdout);
	puts("\033[0m"); // end bold.
	puts("\n[ Loaded commands! ]\n");

	page_file(SCICALC_DOCFILE); /* Init/Help file. */

	/* @todo: Start calculating (i.e. "scicalc"). */
	printf("TODO: Start calculating!\n");

exit:
	delete_sc_stack(stack, free);
	free_command_info();

	return 0;
}

