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
#include "cli.h"

int main(int argc, char *const argv[])
{
	int       opt      = 0, long_index = 0, status = 0;
	FILE     *lf       = stderr; /**< Default logfile. */
	char     *lfpath   = NULL;
	cli_code  cli_stat = CLI_PRE_INIT;

	puts("\033[0;1m"); /* Start bold. */
	puts("\tChemTK SciCalc Version " VERSION ".\n");
	puts("\tDATA_DIR: '" DATA_DIR "'.\n\n");

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

	fputs("Running CLI.\n", lf);
	cli_stat = run_cli(lf);
	fprintf(lf, "CLI exited: %s\n", cli_statuses[cli_stat]);

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
	delete_sc_stack(g_stack, free);
	free_command_info();

	return status;
}

