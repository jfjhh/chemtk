/**
 * @file cli.h
 * @author Alex Striff
 *
 * @brief Main file for ChemTK NCurses CLI.
 */

/* Feature test macro for usleep(3). */
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif /* _BSD_SOURCE */

#ifndef CLI_H
#define CLI_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>
#include <wchar.h>
#include <ncursesw/curses.h>

#include "chemtk.h"

/**
 * The width of the debug and info windows.
 */
#define DBGWIDTH	80

/**
 * The width of the debug window.
 */
#define DBGHEIGHT	24

/**
 * Return codes for the CLI.
 */
typedef enum {
	CLI_PRE_INIT = 0,
	CLI_OK,
	CLI_FAIL_PARSE_CMD_FILE,
	CLI_FAIL_INIT_SC_STACK,
	CLI_FAIL_INIT_CURSES,
} cli_code;

static const char *const cli_statuses[] = {
	[CLI_OK] =
		"OK",
	[CLI_FAIL_PARSE_CMD_FILE] =
		"Could not parse command file (init_sc_commnds)",
	[CLI_FAIL_INIT_SC_STACK] =
		"Could not create the stack (new_sc_stack)",
	[CLI_FAIL_INIT_CURSES] =
		"Could not setup NCurses (bad window sizes).",
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

