#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h> /* DEBUGGING */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define SC_TOKEN_INLEN	64

typedef struct {
	enum {         /* Scalar Type:   */
		NUMBER,    /* double         */
		CONSTANT,  /* double         */
		SIG_FIG,   /* int            */
		CMD,       /* char *         */
		OPERATOR,  /* char           */
		BOOL,      /* bool (_Bool)   */
		NONE       /* uninitialized. */
	} type;
	union {
		double  flt;
		char    op;
		char    cmd[SC_TOKEN_INLEN];
		int     sig_figs;
		bool    b;
	} data;
} sc_token;

#include "command.h"
#include "constant.h"
#include "ldmath.h"
#include "operator.h"

sc_token *sc_tokenize(const char *line);

int test_sc_token(FILE *logfile);

#endif /* TOKEN_H */

