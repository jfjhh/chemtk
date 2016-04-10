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

static void usage(void)
{
	fputs("Usage: `./chemtk <options>`.\n", stderr);
	fputs("\t-l<LOGFILE>\n", stderr);
	fputs("\t\tLog debug and verbose output to a file instead of stderr.\n", stderr);
	fputs("\t-h\n", stderr);
	fputs("\t\tThis help.\n", stderr);
}

int main(int argc, char *const argv[])
{
	int       opt      = 0, long_index = 0, status = 0;
	FILE     *lf       = stderr; /**< Default logfile. */
	char     *lfpath   = NULL;
	cli_code  cli_stat = CLI_PRE_INIT;

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
				if (!(lf = freopen(optarg, "a+", stderr))) {
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
				fprintf(stderr, "\t*** Logfile: '%s' ***\n", lfpath);
				break;
			case 'h':
				usage();
				goto exit;
			default:
				break;
		}
	}

	if (!lfpath) {
		char *tmpname = strdup("/tmp/chemtk_dbg_XXXXXX");
		assert(tmpname);
		int tmpfile = mkstemp(tmpname);
		lfpath = tmpname;
		if (tmpfile == -1) {
			perror("mkstemp");
			goto exit;
		}
		if (!(lf = freopen(tmpname, "a+", stderr))) {
			fprintf(stderr, "Could not open logfile '%s'!\n", tmpname);
			perror("logfile");
			status = 2;
			goto exit;
		}
	}

	cli_stat = run_cli(stderr);
	printf("CLI exited: %s\n", cli_statuses[cli_stat]);
	if (cli_stat != CLI_OK)
		status = 100 + cli_stat;

exit:
	if (lf != stderr && lf != NULL)
		fclose(lf);
	if (lfpath) {
		if (unlink(lfpath) == -1) {
			fprintf(stderr, "Could not unlink '%s'.\n", lfpath);
			perror("unlink");
		}
	}
	free(lfpath);
	delete_sc_stack(g_stack, free);
	free_command_info();

	return status;
}

