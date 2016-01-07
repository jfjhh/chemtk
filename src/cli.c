/**
 * @file cli.h
 * @author Alex Striff
 *
 * @brief Main file for ChemTK NCurses CLI.
 */

#include "cli.h"

cli_code run_cli(FILE *logfile)
{
	/* Parse command file. */
	puts("[ Parsing Command File... ]\n");
	puts("\033[0m"); /* End bold. */
	if (parse_command_file(COMMAND_FILE, logfile) != 0) { /* Init. commands. */
		return CLI_FAIL_PARSE_CMD_FILE;
	} else if (!(g_stack = new_sc_stack())) { /* Init. stack. */
		return CLI_FAIL_INIT_SC_STACK;
	}

	puts("\033[0;1m"); /* Start bold. */
	print_sc_commands(stdout);
	puts("\033[0m"); /* End bold. */
	puts("\n[ Loaded commands! ]\n");

	page_file(SCICALC_DOCFILE); /* Init. and Help file. */

	/* @todo: Start calculating (i.e. "scicalc"). */
	printf("TODO: Start calculating!\n");

	return CLI_OK;
}

