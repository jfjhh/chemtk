/**
 * @file scicalc.c
 * @author Alex Striff
 *
 * @brief Allows RPN style operation on a stack.
 */

#include "scicalc.h"

int operate(sc_stack **stack)
{
	char      operation;
	sc_token *a, *b, *c, *op;
	a = b = c = op = NULL;

	/* Check the operator from the stack for validity. */
	if (!(op = peek_sc_stack(stack))
			|| SCT_TYPE(op) != OPERATOR
			|| !is_operator(SCT_OP(op))) {
		fprintf(stderr, "\tInvalid operator '%c':\n", SCT_OP(op));
		print_sc_token(stderr, op);
		return 0;
	}

	/* Get the operator char. */
	op        = pop_sc_stack(stack);
	operation = SCT_OP(op);
	free(op);

	if (!(c = calloc(1, sizeof(sc_token)))) {
		fputs("Could not allocate result token!", stderr);
		return 0;
	}

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
	if (!a || !b)
		return 0;

	/* Get sig. figs. for result based on operation. */
	switch (operation) {
		case '+':
		case '-': /* Use sig. figs. after decimal place. */
			SCT_FIG(c, AFTER) = least(SCT_FIG(a, AFTER), SCT_FIG(b, AFTER));
			break;
		default: /* Use all sig. figs. ('*' and '/', especially). */
			SCT_FIG(c, ALL)   = least(SCT_FIG(a, ALL),   SCT_FIG(b, ALL));
			break;
	}

	/* Actually do the operation. */
	SCT_TYPE(c) = VALUE;
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
			SCT_FLT(c) = (SCT_FLT(a) > SCT_FLT(b))  ? 1.0L  : 0.0L;
			break;
		case '<':
			SCT_FLT(c) = (SCT_FLT(a) < SCT_FLT(b))  ? 1.0L  : 0.0L;
			break;
		case '=':
			SCT_FLT(c) = (SCT_FLT(a) == SCT_FLT(b)) ? 1.0L  : 0.0L;
			break;
		case '^':
			SCT_FLT(c) = powl(SCT_FLT(a), SCT_FLT(b));
			break;
		case 'o':
			SCT_FLT(c) = logl(SCT_FLT(a));
			break;
		default:
			SCT_FLT(c)  = 0.0L;
			SCT_TYPE(c) = NONE;
			SIG_FIG_SET(SCT_FIGS(c), 0);
			break;
	}

	/* Free a and b */
	if (b != a)
		free(b);
	free(a);

	/* Set boolean value */
	SCT_BOOL(c) = (SCT_FLT(c) == 0.0) ? false : true;

	/* DEBUG */
	fputs("\tOperated and created a new token:\n", stderr);
	print_sc_token(stderr, c);
	fputc('\n', stderr);

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
	int       status        = 0;
	sc_stack *stack         = NULL;
	sc_token *token         = NULL;
	char     *test_tokens[] = { "13", "37", "+", NULL }; /* Does `13 + 37`. */

	fputs("TODO: write test_scicalc().\n", logfile);

	if (!(stack = new_sc_stack())) {
		fputs("Could not create a stack to test scicalc!\n", logfile);
		return 0;
	}

	/* Add test_tokens to the stack as actual tokens. */
	for (size_t i = 0; test_tokens[i]; i++) {
		if (!(token = sc_tokenize(test_tokens[i]))) {
			fputs("Created NULL token!\n", logfile);
			goto exit;
		}
		if (!push_sc_stack(&stack, token)) {
			fputs("Could not push token to stack!\n", logfile);
			goto exit;
		}
	}

	/* Print before. */
	fputs("\tBefore operate():\n", logfile);
	print_sc_stack(logfile, stack, (sc_stack_pfun) print_sc_token);

	/* Operate() and print after. */
	status = operate(&stack);
	fputs("\n\tAfter operate():\n", logfile);
	print_sc_stack(logfile, stack, (sc_stack_pfun) print_sc_token);

	/* Check output token on stack. */
	token = peek_sc_stack(&stack);
	fprintf(logfile, "Double precision floating-point error: %Lf\n"
			"LDBL_EPSILON: %Lf\n",
			SCT_FLT(token) - 50.0L, LDBL_EPSILON);
	if (!(status += LD_EQUAL(SCT_FLT(token), 50.0L)))
		fputs("Floating point result found to be not equal!\n", logfile);
	if (!(status += (SCT_TYPE(token) != VALUE) ? 0 : 1))
		fputs("Type of token was not VALUE.\n", logfile);

	fprintf(logfile, "Status is %d.\n", status);

exit: /* Fallthrough on success. */
	delete_sc_stack(stack, free);
	return (status != 0 ? 1 : 0);
}

