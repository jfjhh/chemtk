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

	if (sscanf(line, "%le", &flt) == 1) { /* type is VALUE. */
		SCT_TYPE(result_token) = VALUE;
		SCT_FLT(result_token) = flt;
		if (sig_figs(line, SCT_FIGS(result_token)) <= 0)
			fputs("sc_tokenize(): could not get sig. figs.\n", stderr);
	} else if (is_operator(line[0])) { /* type is OPERATOR. */
		SCT_TYPE(result_token) = OPERATOR;
		SCT_OP(result_token) = line[0];
	} else if (is_sc_command(line)) { /* type is CMD. */
		fputs("Token type is CMD.\n", stderr);
		if (!run_sc_command(line, result_token, stderr)) {
			fputs("sc_tokenize(): failed to run command.\n", stderr);
			SCT_TYPE(result_token) = NONE;
		} else {
			fputs("sc_tokenize(): command successful.\n", stderr);
		}
	} else {
		fputs("sc_tokenize(): invalid token.", stderr);
		free(result_token);
		return NULL;
	}

	fputs("sc_tokenize(): valid token.", stderr);
	return result_token;
}

/* sc_token *sc_tokenize(const char *line) */
/* { */
/* 	int i, is_num; */
/* 	long double tmp; */
/* 	sc_token *token; */

/* 	if (!(token = malloc(sizeof(sc_token)))) */
/* 		return NULL; */

/* 	i = is_num  = 0; */
/* 	token->type = NONE; */
/* 	memset(token->data.cmd, '\0', CMD_LINELEN); /1* zero data. *1/ */

/* 	for (i = 0; i < 10; i++) { */
/* 		if (line[0] == '0' + i) { */
/* 			sscanf(line, "%le", &(token->data.flt)); */

/* 			if (line[CMD_LINELEN - 2] == SC_CONST_CHAR) */
/* 				token->type = CONSTANT; */
/* 			else */
/* 				token->type = NUMBER; */

/* 			token->data.sig_figs = sig_figs(line); */
/* 			is_num               = 1; */

/* 			break; */
/* 		} */
/* 	} */

/* 	if (!is_num) { */
/* 		if ((tmp = const_search(line)) != 0) { */
/* 			token->type = CONSTANT; */
/* 		} else if ((tmp = is_operator(line[0]))) { */
/* 			fprintf(stderr, "sc_tokenize() got an OPERATOR.\n"); */
/* 			token->type = OPERATOR; */
/* 		} else { */
/* 			token->type = CMD; */
/* 		} */
/* 		token->data.flt = tmp; */
/* 	} */

/* 	strncpy(token->data.cmd, line, CMD_LINELEN); */
/* 	token->data.cmd[CMD_LINELEN - 1] = '\0'; /1* Force NULL termination. *1/ */

/* 	fprintf(stderr, "Created sc_token: %p.\n", (void *) token); */

/* 	return token; */
/* } */

/* int sc_token_figs(sc_token token, sig_fig_field field) */
/* { */
/* 	if (token */
/* 			&& token.type == VALUE */
/* 			&& field < SIG_FIG_LEN */
/* 			&& field >= 0) */
/* 		return token.data.value.figs[field]; */
/* 	else */
/* 		return -2; */
/* } */

void print_sc_token(sc_token *token, FILE *file)
{
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

int test_sc_token(FILE *logfile)
{
	/* Command tree initialized in previous test. */
	int status;
	status = 0;
	sc_token *out;

	fputs("TODO: write test_sc_token().\n", logfile);

	if (!(out = sc_tokenize("T"))) {
		fputs("Could not tokenize!", stderr);
	} else {
		print_sc_token(out, logfile);
		free(out);
		status = 1;
	}

	free(sc_inputs);
	free_command_tree(sc_commands);

	return status;
}

