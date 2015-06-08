/**
 * @file token.c
 * @author Alex Striff
 *
 * @brief Provides a generic stack member datatype: a parsed token.
 */

#include "token.h"

/**
 * @todo handle_fields will become a part of @c sc_tokenize() (this includes
 * rejecting an invalid input string.
 */
static int handle_fields(WINDOW *outwin, struct num_str *numstr,
		struct stack *stack)
{
	int status = 1;

	/* Be sure that this is a cmd or operation. */
	if (numstr->type == CMD || numstr->type == OPERATOR) {
		wprintw(outwin, "handle_fields got a CMD or OPERATOR type numstr.\n");
		status = run_cmd(outwin, numstr, stack);
	} else {
		/* @todo Check for a valid number using regexp. */
	}

	if (status && numstr->type != CMD) /* cmd resulted in a number */
		push_stack(&stack, numstr);
	else
		free(numstr);

	return status;
}

sc_token *sc_tokenize(const char *line)
{
	int i, is_num;
	long double tmp;
	sc_token *token;

	if (!(token = (sc_token *) malloc(sizeof(sc_token))))
		return NULL;

	i = is_num  = 0;
	token->type = NONE;
	memset(token->data.cmd, '\0', CMD_LINELEN); /* zero data. */

	for (i = 0; i < 10; i++) {
		if (line[0] == '0' + i) {
			sscanf(line, "%le", &(token->data.flt));

			if (line[CMD_LINELEN - 2] == SC_CONST_CHAR)
				token->type = CONSTANT;
			else
				token->type = NUMBER;

			token->data.sig_figs = sig_figs(line);
			is_num               = 1;

			break;
		}
	}

	if (!is_num) {
		if ((tmp = const_search(line)) != 0) {
			token->type = CONSTANT;
		} else if ((tmp = is_operator(line[0]))) {
			fprintf(stderr, "sc_tokenize() got an OPERATOR.\n");
			token->type = OPERATOR;
		} else {
			token->type = CMD;
		}
		token->data.flt = tmp;
	}

	strncpy(token->data.cmd, line, CMD_LINELEN);
	token->data.cmd[CMD_LINELEN - 1] = '\0'; /* Force NULL termination. */

	fprintf(stderr, "Created sc_token: %p.\n", (void *) token);

	return token;
}

/* @test Write test_sc_token(). */
int test_sc_token(FILE *logfile)
{
	int status;
	status = 0;

	fprintf(logfile, "TODO: write test_sc_token().\n");

	return status;
}

