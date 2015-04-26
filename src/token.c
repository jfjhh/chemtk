#include "token.h"

/* TODO: handle_fields will become a part of tokenizing (this includes rejecting
 * an invalid input string. */

/* static int handle_fields(WINDOW *outwin, struct num_str *numstr, */
/* 		struct stack *stack) */
/* { */
/* 	int status = 1; */

/* 	/1* Be sure that this is a cmd or operation. *1/ */
/* 	if (numstr->type == CMD || numstr->type == OPERATOR) { */
/* 		wprintw(outwin, "handle_fields got a CMD or OPERATOR type numstr.\n"); */
/* 		status = run_cmd(outwin, numstr, stack); */
/* 	} else { */
/* 		/1* TODO: Check for a valid number using regexp. *1/ */
/* 	} */

/* 	if (status && numstr->type != CMD) /1* cmd resulted in a number *1/ */
/* 		push_stack(&stack, numstr); */
/* 	else */
/* 		free(numstr); */

/* 	return status; */
/* } */

/* TODO: write sc_tokenize(). */
sc_token *sc_tokenize(const char *line)
{
	int i, is_num;
	long double tmp;
	sc_token *token;

	if (!(token = (sc_token *) malloc(sizeof(sc_token))))
		return NULL;

	i = is_num  = 0;
	token->type = NONE;
	memset(token->data.cmd, '\0', SC_TOKEN_INLEN); /* zero data. */

	for (i = 0; i < 10; i++) {
		if (line[0] == '0' + i) {
			sscanf(line, "%le", &(token->data.flt));

			if (line[SC_TOKEN_INLEN - 2] == SC_CONST_CHAR)
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

	strncpy(token->data.cmd, line, SC_TOKEN_INLEN);
	token->data.cmd[SC_TOKEN_INLEN - 1] = '\0'; /* Force NULL termination. */

	fprintf(stderr, "Created sc_token: %p.\n", (void *) token);

	return token;
}

/* TODO: write test_sc_token(). */
int test_sc_token(FILE *logfile)
{
	int status;
	status = 0;

	fprintf(logfile, "TODO: write test_sc_token().\n");

	return status;
}

