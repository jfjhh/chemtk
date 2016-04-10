/**
 * @file token.c
 * @author Alex Striff
 *
 * @brief Provides a generic stack member datatype: a parsed token.
 */

#include "token.h"

sc_token *sc_tokenize(char *line)
{
	sc_token *result_token;
	double flt;

	if (!(result_token = calloc(sizeof(sc_token), 1))) {
		fputs("sc_tokenize(): failed to allocate memory for sc_token.\n",
				stderr);
		return NULL;
	}

	if (is_operator(line[0])) {                  /* Type is OPERATOR. */
		SCT_TYPE(result_token) = OPERATOR;
		SCT_OP(result_token)   = line[0];
	} else if (sscanf(line, "%le", &flt) == 1) { /* Type is VALUE.    */
		SCT_TYPE(result_token) = VALUE;
		SCT_FLT(result_token)  = flt;
		if (sig_figs(line, SCT_FIGS(result_token)) <= 0)
			fputs("sc_tokenize(): could not get sig. figs.\n", stderr);
	} else if (is_sc_command(line)) {            /* Type is CMD.      */
		fputs("Token type is CMD.\n", stderr);
		if (!run_sc_command(line, result_token, stderr)) { /* Try it! */
			fputs("sc_tokenize(): failed to run command.\n", stderr);
			SCT_TYPE(result_token) = NONE;
		} else {                                 /* Successful CMD.   */
			fputs("sc_tokenize(): command successful.\n", stderr);
		}
	} else {                                     /* Invalid token.    */
		fputs("sc_tokenize(): invalid token.", stderr);
		free(result_token);
		return NULL;
	}

	return result_token;
}

void print_sc_token(FILE *file, sc_token *token)
{
	if (!token)
		fputs("< NULL Token > :: NONE\n", file);

	switch (SCT_TYPE(token)) {
		case VALUE:
			fprintf(file, "< %.*f (%s) > :: VALUE\n",
					SCT_FIG(token, AFTER),
					SCT_FLT(token),
					SCT_BOOL(token) ? "true" : "false");
			break;
		case CMD:
			fprintf(file, "< `%s` > :: CMD\n",
					SCT_CMD(token));
			break;
		case OPERATOR:
			fprintf(file, "< '%c' > :: OPERATOR\n",
					SCT_OP(token));
			break;
		case NONE:
		default:
			fputs("< Empty Token > :: NONE\n", file);
			break;
	}
}

void sc_wprint_token(WINDOW *win, void *data)
{
	if (!data) {
		wprintw(win, "< NULL Token > :: NONE\n");
		return;
	}
	sc_token *token = (sc_token *) data;

	switch (SCT_TYPE(token)) {
		case VALUE:
			wprintw(win, "< %.*f (%s) > :: VALUE\n",
					SCT_FIG(token, AFTER),
					SCT_FLT(token),
					SCT_BOOL(token) ? "true" : "false");
			break;
		case CMD:
			wprintw(win, "< `%s` > :: CMD\n",
					SCT_CMD(token));
			break;
		case OPERATOR:
			wprintw(win, "< '%c' > :: OPERATOR\n",
					SCT_OP(token));
			break;
		case NONE:
		default:
			wprintw(win, "< Empty Token > :: NONE\n");
			break;
	}
}

int test_sc_token(FILE *logfile)
{
	/**
	 * @test Tests tokenization of an input line (executes test command).
	 */

	/* The command tree was initialized in a previous test. */
	int status;
	status = 0;
	sc_token *out;

	if (!(out = sc_tokenize("T"))) {
		fputs("Could not tokenize!", stderr);
	} else {
		print_sc_token(logfile, out);
		free(out);
		status = 1;
	}

	free(sc_inputs);
	free_command_tree(sc_commands);

	return status;
}

