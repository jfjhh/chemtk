/**
 * @file command.c
 * @author Alex Striff
 *
 * @brief Runs commands that produce numbers to calculate with.
 *
 * An example of a command's output would be the atomic mass of an element from
 * the periodic table.
 */

#include "command.h"

int init_sc_commands(void)
{
	sc_commands = (struct sc_command_table *) \
				  malloc(sizeof(struct sc_command_table));

	if (!sc_commands) {
		fprintf(stderr,
				"init_sc_commands(): could not allocate sc_command_table.\n");
		return 0;
	}

	sc_commands->entries = NULL;
	sc_commands->count = 0;

	return 1;
}

int add_sc_command(sc_command_fun *cmd_fun, enum sc_command_type type)
{
	sc_commands->count++;

	sc_commands->entries = realloc(sc_commands->entries,
			sc_commands->count * (sizeof(struct sc_command_entry)));

	if (!sc_commands->entries) {
		fprintf(stderr,
				"Failed to realloc() sc_commands->entries. Original untouched.");
		sc_commands->count--;
		return 0;
	}

	sc_commands->entries[sc_commands->count -1].type = type;
	sc_commands->entries[sc_commands->count -1].cmd_fun = cmd_fun;

	return 1;
}

void free_sc_commands(void)
{
	free(sc_commands->entries);
	free(sc_commands);
}

enum sc_command_type is_sc_command(const char *line)
{
	enum sc_command_type type;

	switch (line[0]) {
		case ELEMENT_CMD:
			type = ELEMENT_CMD;
			break;
		case CONSTANT_CMD:
			type = CONSTANT_CMD;
			break;
		default:
			type = NOT_CMD;
			break;
	}

	return type;
}

int run_sc_command(const char *line, sc_token *token, FILE *logfile)
{
	/**
	 * @todo write run_sc_command().
	 */
	enum sc_command_type type = line[0];

	/* Search through sc_commands for type, and execute that function. */

	return 0;
}

static int test_sc_command_fun(const char *line, sc_token *token, FILE *logfile)
{
	fprintf(logfile, "test_sc_command_fun():\n");
	fprintf(logfile, "\tGot line: '%s'\n", line);
	fprintf(logfile, "\tGot token: '%p'\n", (void *) token);

	SCT_TYPE(token) = VALUE;
	fprintf(logfile, "\tSet token type to VALUE.\n");

	return 1;
}

int test_command(FILE *logfile)
{
	sc_token *test_token = (sc_token *) malloc(sizeof(sc_token));

	if (!test_token) {
		fprintf(logfile, "Could not create a testing sc_token.\n");
		return 0;
	}

	if (!init_sc_commands()) {
		fprintf(logfile, "Could not init_sc_commands().\n");
		return 0;
	}

	if (!add_sc_command(test_sc_command_fun, ELEMENT_CMD)) {
		fprintf(logfile, "Could not add_sc_command().\n");
		return 0;
	}

	if (!run_sc_command(test_sc_command_fun, test_token, logfile)) {
		fprintf(logfile, "Could not run_sc_command() [failed].\n");
		return 0;
	}

	if (SCT_TYPE(test_token) != VALUE) { /* Set by the command as a check. */
		fprintf(logfile, "Could not run_sc_command() [failed].\n");
		return 0;
	}

	free_sc_commands();
	free(test_token);

	return 1;
}

