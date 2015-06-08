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

	/* Get the op from the stack and free it */
	if (!(op = pop_stack(&stack)))
		return 0;

	operation = op->data.op;
	free(op);

	if (!(c = (sc_token *) malloc(sizeof(sc_token))))
		return 0;

	/* Pop from the stack. */
	if (operation != '~') {
		/* This is the only unary op, so all the others are binary and need two
		 * sc_tokens to be popped. */
		a = pop_stack(&stack);
		b = pop_stack(&stack);
	} else { /* Operation is unary. */
		a = pop_stack(&stack);
		b = a;
	}

	/* Check pop(s) went alright. */
	if (!(a && b))
		return 0;

	/* Get sig. figs. for result based on operation. */
	switch (operation) {
		case '+':
		case '-':
			c->sig_figs = least(sig_after(a->str), sig_after(b->str));
			break;
		default: /* Same as for multiply/divide, and other ops. */
			c->sig_figs = least(a->sig_figs, b->sig_figs);
			break;
	}

	switch (operation) {
		case '+':
			c->data = a->data + b->data;
			break;
		case '-':
			c->data = a->data - b->data;
			break;
		case '*':
			c->data = a->data * b->data;
			break;
		case '/':
			c->data = a->data / b->data;
			break;
		case '%':
			c->data = DTOI(a->data) %  DTOI(b->data);
			break;
		case '&':
			c->data = DTOI(a->data) &  DTOI(b->data);
			break;
		case '|':
			c->data = DTOI(a->data) |  DTOI(b->data);
			break;
		case 'x':
			c->data = DTOI(a->data) ^  DTOI(b->data);
			break;
		case 'l':
			c->data = DTOI(a->data) << DTOI(b->data);
			break;
		case 'r':
			c->data = DTOI(a->data) >> DTOI(b->data);
			break;
		case '>' :
			c->data = (a->data > b->data) ? 1.0L  : 0.0L;
			break;
		case '<':
			c->data = (a->data < b->data) ? 1.0L  : 0.0L;
			break;
		case '=':
			c->data = (a->data == b->data) ? 1.0L : 0.0L;
			break;
		case '^':
			c->data = powl(a->data, b->data);
			break;
		case 'o':
			c->data = logl(a->data);
			break;
		default:
			c->data = 0.0L;
			break;
	}

	/* Free a and b */
	if (b == a) {
		free(a);
	} else {
		free(a);
		free(b);
	}

	/* Fix c->str */
	if (strbool) {
		if (c->data == 1.0L)
			strncpy(c->str,  "True",   SC_TOKEN_INLEN);
		else
			strncpy(c->str,  "False",  SC_TOKEN_INLEN);
	} else {
		sscanf(c->str, "%Le", &(c->data));
	}

	fprintf(logfile, "Got: %s (%Le)", c->str, c->data);
	push_stack(&stack, c);

	return 1;
}

/* @test Write test_scicalc(). */
int test_scicalc(FILE *logfile)
{
	fprintf(logfile, "TODO: write test_scicalc().\n");
	return 0;
}

