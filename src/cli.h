/**
 * @file cli.h
 * @author Alex Striff
 *
 * @brief Main file for ChemTK NCurses CLI.
 */

#ifndef CLI_H
#define CLI_H

#include <stdio.h>
#include <ncurses.h>

#include "chemtk.h"

/**
 * Return codes for the CLI.
 */
typedef enum {
	CLI_PRE_INIT = 0,
	CLI_OK,
	CLI_FAIL_PARSE_CMD_FILE,
	CLI_FAIL_INIT_SC_STACK,
} cli_code;

static const char *const cli_statuses[] = {
	[CLI_OK] =
		"OK",
	[CLI_FAIL_PARSE_CMD_FILE] =
		"Could not parse command file (init_sc_commnds)",
	[CLI_FAIL_INIT_SC_STACK] =
		"Could not create the stack (new_sc_stack)",
};

/**
 * The global stack.
 */
static sc_stack *g_stack = NULL;

/**
 * Runs ChemTK with a NCurses CLI.
 *
 * @param logfile The logfile for internal routines.
 *
 * @returns cli_code The exit code of the CLI.
 */
cli_code run_cli(FILE *logfile);

#endif /* CLI_H */

