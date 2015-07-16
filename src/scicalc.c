/**
 * @file scicalc.c
 * @author Alex Striff
 *
 * @brief Allows RPN style operation on a stack.
 */

#include "scicalc.h"

int operate(sc_stack **stack)
{
	char operation;
	sc_token *a, *b, *c, *op;
	a = b = c = op = NULL;

	/* Get the op from the stack and free it (and check for validity). */
	if (!(op = pop_sc_stack(stack))
			|| SCT_TYPE(op) == OPERATOR
			|| !is_operator(SCT_OP(op))) {
		push_sc_stack(stack, op); /* Undo changes to stack. */
		return 0;
	}

	operation = SCT_OP(op);
	free(op);

	if (!(c = (sc_token *) malloc(sizeof(sc_token))))
		return 0;

	/* Pop from the stack. */
	if (operation != '~') {
		/* This is the only unary op, so all the others are binary and need two
		 * sc_tokens to be popped. */
		a = pop_sc_stack(stack);
		b = pop_sc_stack(stack);
	} else { /* Operation is unary. */
		a = pop_sc_stack(stack);
		b = a;
	}

	/* Check pop(s) went alright. */
	if (!(a && b))
		return 0;

	/* Get sig. figs. for result based on operation. */
	switch (operation) {
		case '+':
		case '-': /* Use sig. figs. after decimal place. */
			SCT_FIG(c, AFTER) = least(SCT_FIG(a, AFTER), SCT_FIG(b, AFTER));
			break;
		default: /* Use all sig. figs.
					Same for multiply/divide as for other ops. */
			SCT_FIG(c, ALL) = least(SCT_FIG(a, ALL), SCT_FIG(b, ALL));
			break;
	}

	switch (operation) {
		case '+':
			SCT_FLT(c) = SCT_FLT(a) + SCT_FLT(b);
			break;
		case '-':
			SCT_FLT(c) = SCT_FLT(a) - SCT_FLT(b);
			break;
		case '*':
			SCT_FLT(c) = SCT_FLT(a) * SCT_FLT(b);
			break;
		case '/':
			SCT_FLT(c) = SCT_FLT(a) / SCT_FLT(b);
			break;
		case '%':
			SCT_FLT(c) = DTOI(SCT_FLT(a)) %  DTOI(SCT_FLT(b));
			break;
		case '&':
			SCT_FLT(c) = DTOI(SCT_FLT(a)) &  DTOI(SCT_FLT(b));
			break;
		case '|':
			SCT_FLT(c) = DTOI(SCT_FLT(a)) |  DTOI(SCT_FLT(b));
			break;
		case 'x':
			SCT_FLT(c) = DTOI(SCT_FLT(a)) ^  DTOI(SCT_FLT(b));
			break;
		case 'l':
			SCT_FLT(c) = DTOI(SCT_FLT(a)) << DTOI(SCT_FLT(b));
			break;
		case 'r':
			SCT_FLT(c) = DTOI(SCT_FLT(a)) >> DTOI(SCT_FLT(b));
			break;
		case '>' :
			SCT_FLT(c) = (SCT_FLT(a) > SCT_FLT(b)) ? 1.0L  : 0.0L;
			break;
		case '<':
			SCT_FLT(c) = (SCT_FLT(a) < SCT_FLT(b)) ? 1.0L  : 0.0L;
			break;
		case '=':
			SCT_FLT(c) = (SCT_FLT(a) == SCT_FLT(b)) ? 1.0L : 0.0L;
			break;
		case '^':
			SCT_FLT(c) = powl(SCT_FLT(a), SCT_FLT(b));
			break;
		case 'o':
			SCT_FLT(c) = logl(SCT_FLT(a));
			break;
		default:
			SCT_FLT(c) = 0.0L;
			SIG_FIG_SET(SCT_FIGS(c), 0);
			SCT_TYPE(c) = NONE;
			break;
	}

	/* Free a and b */
	if (b == a) {
		free(a);
	} else {
		free(a);
		free(b);
	}

	/* Set boolean value */
	SCT_BOOL(c) = (SCT_FLT(c) == 0.0) ? false : true;

	/* ALL sig. figs. -1 because of the first number. E.g. '1.23' with
	 * total figs. 3 - 1 == 2 figs.  after the decimal. */
	fprintf(stderr, "Got: %- .*e).",
			SCT_FIG(c, ALL) - 1, SCT_FLT(c));
	push_sc_stack(stack, c);

	return 1;
}

/* @test Write test_scicalc(). */
int test_scicalc(FILE *logfile)
{
	fprintf(logfile, "TODO: write test_scicalc().\n");
	return 0;
}

