/**
 * @file chemtk.c
 * @author Alex Striff
 *
 * @brief Main file for program execution.
 */

#if ! defined(_XOPEN_SOURCE) || _XOPEN_SOURCE < 700 /* For strndup(3). */
#define _XOPEN_SOURCE	700
#endif /* _XOPEN_SOURCE */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "chemtk.h"
#include "arguments.h"

int main(int argc, char *const argv[])
{
	/**
	 * @todo Parse command line args (@c getopt(3)).
	 * - The log file, which is stderr by default.
	 */
	int       opt      = 0, long_index = 0, status = 0;
	sc_stack *stack    = NULL;
	FILE     *lf       = stderr; /**< Default logfile. */
	char     *lfpath   = NULL;

	puts("\033[0;1m"); /* Start bold. */
	puts("ChemTK SciCalc Version " VERSION ".\n\n");

	/* Parse command-line arguments. */
	while ((opt = getopt_long(argc, argv,
					g_short_opts, g_long_opts, &long_index)) != -1) {
		switch (opt) {
			case 'l':
				if (!optarg) {
					fputs("The logfile option requires a valid argument.\n",
							stderr);
					status = 1;
					goto exit;
				}
				if (!(lf = fopen(optarg, "a+"))) {
					fprintf(stderr, "Could not open logfile '%s'!\n", optarg);
					perror("logfile");
					status = 2;
					goto exit;
				}
				if (!(lfpath = strndup(optarg, strlen(optarg) + 1))) {
					perror("logfile path strndup");
					status = 3;
					goto exit;
				}
				fprintf(lf, "\t*** Logfile: '%s' ***\n", lfpath);
				break;
			default:
				break;
		}
	}

	fflush(lf);
	getchar(); /* DEBUG pause. */
	goto exit;

	/* Parse command file. */
	puts("[ Parsing Command File... ]\n");
	puts("\033[0m"); /* End bold. */
	if (parse_command_file(COMMAND_FILE, lf) != 0) { /* Init. commands. */
		fputs("Failed to parse command file (init sc_commands)!\n", lf);
		goto exit;
	} else if (!(stack = new_sc_stack())) { /* Init. stack. */
		fputs("Failed to init sc_stack!\n", lf);
		free_command_info();
		goto exit;
	}

	puts("\033[0;1m"); /* Start bold. */
	print_sc_commands(stdout);
	puts("\033[0m"); /* End bold. */
	puts("\n[ Loaded commands! ]\n");

	page_file(SCICALC_DOCFILE); /* Init. and Help file. */

	/* @todo: Start calculating (i.e. "scicalc"). */
	printf("TODO: Start calculating!\n");

exit:
	if (lf != stderr && lf != NULL) {
		fclose(lf);
		if (lfpath) {
			if (unlink(lfpath) == -1) {
				fprintf(stderr, "Could not unlink '%s'.\n", lfpath);
				perror("unlink");
			}
		}
	}
	free(lfpath);
	delete_sc_stack(stack, free);
	free_command_info();

	return status;
}

