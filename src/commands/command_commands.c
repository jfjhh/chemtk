#include "command_commands.h"

	__attribute__((unused))
int test_sc_command_fun(char *line, sc_token *token, FILE *logfile)
{
	if (!line || !token || !logfile)
		return 0;

	fputs("test_sc_command_fun():\n", logfile);
	fprintf(logfile, "\tGot line: '%s'\n", line);
	fprintf(logfile, "\tGot token: '%p'\n", (void *) token);

	SCT_TYPE(token) = VALUE;
	fprintf(logfile, "\tSet token type to VALUE.\n");

	return 1;
}


