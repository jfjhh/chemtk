#include "scicalc.h"

/* TODO: write operate() to change stack in-place. */

/* int operate(struct stack **stack) */
/* { */
/* 	char operation; */
/* 	struct sc_token *a, *b, *c, *op; */
/* 	int strbool; */

/* 	a = b = c = op = NULL; */
/* 	strbool        = 0; */

/* 	/1* Get the op from the stack and free it *1/ */
/* 	if (!(op = pop_stack(&stack))) */
/* 		return 0; */

/* 	operation = op->str[0]; */
/* 	free(op); */

/* 	if (!(c = (struct sc_token *) malloc(sizeof(struct sc_token)))) */
/* 		return 0; */

/* 	/1* Pop from the stack. *1/ */
/* 	if (operation != '~') { */
/* 		/1* This is the only unary op, so all the others are binary and need two */
/* 		 * sc_tokens to be popped. *1/ */
/* 		a = pop_stack(&stack); */
/* 		b = pop_stack(&stack); */
/* 	} else { /1* Operation is unary. *1/ */
/* 		a = pop_stack(&stack); */
/* 		b = a; */
/* 	} */

/* 	/1* Check pop(s) went alright. *1/ */
/* 	if (!(a && b)) */
/* 		return 0; */

/* 	/1* Get sig. figs. for result based on operation. *1/ */
/* 	switch (operation) { */
/* 		case '+': */
/* 		case '-': */
/* 			c->sig_figs = least(sig_after(a->str), sig_after(b->str)); */
/* 			break; */
/* 		default: /1* Same as for multiply/divide, and other ops. *1/ */
/* 			c->sig_figs = least(a->sig_figs, b->sig_figs); */
/* 			break; */
/* 	} */

/* 	switch (operation) { */
/* 		case '+': */
/* 			c->data = a->data + b->data; */
/* 			break; */
/* 		case '-': */
/* 			c->data = a->data - b->data; */
/* 			break; */
/* 		case '*': */
/* 			c->data = a->data * b->data; */
/* 			break; */
/* 		case '/': */
/* 			c->data = a->data / b->data; */
/* 			break; */
/* 		case '%': */
/* 			c->data = LDTOI(a->data) %  LDTOI(b->data); */
/* 			break; */
/* 		case '&': */
/* 			c->data = LDTOI(a->data) &  LDTOI(b->data); */
/* 			break; */
/* 		case '|': */
/* 			c->data = LDTOI(a->data) |  LDTOI(b->data); */
/* 			break; */
/* 		case 'x': */
/* 			c->data = LDTOI(a->data) ^  LDTOI(b->data); */
/* 			break; */
/* 		case 'l': */
/* 			c->data = LDTOI(a->data) << LDTOI(b->data); */
/* 			break; */
/* 		case 'r': */
/* 			c->data = LDTOI(a->data) >> LDTOI(b->data); */
/* 			break; */
/* 		case '>' : */
/* 			c->data = (a->data > b->data) ? 1.0L  : 0.0L; */
/* 			break; */
/* 		case '<': */
/* 			c->data = (a->data < b->data) ? 1.0L  : 0.0L; */
/* 			break; */
/* 		case '=': */
/* 			c->data = (a->data == b->data) ? 1.0L : 0.0L; */
/* 			break; */
/* 		case '^': */
/* 			c->data = powl(a->data, b->data); */
/* 			break; */
/* 		case 'o': */
/* 			c->data = logl(a->data); */
/* 			break; */
/* 		default: */
/* 			c->data = 0.0L; */
/* 			break; */
/* 	} */

/* 	/1* Free a and b *1/ */
/* 	if (b == a) { */
/* 		free(a); */
/* 	} else { */
/* 		free(a); */
/* 		free(b); */
/* 	} */

/* 	/1* Fix c->str *1/ */
/* 	if (strbool) { */
/* 		if (c->data == 1.0L) */
/* 			strncpy(c->str,  "True",   NUMSTR_BUFSIZE); */
/* 		else */
/* 			strncpy(c->str,  "False",  NUMSTR_BUFSIZE); */
/* 	} else { */
/* 		sscanf(c->str, "%Le", &(c->data)); */
/* 	} */

/* 	fprintf(logfile, "Got: %s (%Le)", c->str, c->data); */
/* 	push_stack(&stack, c); */

/* 	return 1; */
/* } */

/* TODO: write test_scicalc(). */
int test_scicalc(FILE *logfile)
{
	fprintf(logfile, "TODO: write test_scicalc().\n");
	return 0;
}

/* void scicalc(FILE *logfile) */
/* { */
/* 	sc_token *token; */
/* 	sc_stack *stack; */
/* 	int ok, valid; */
/* 	ok = valid = 1; */

/* 	/1* Allocate stack. *1/ */
/* 	if (!(stack = new_stack())) { */
/* 		fprintf(logfile, "Could not create a new struct stack *! (0)\n"); */
/* 		ok = 0; */
/* 	} */

/* 	/1* Elements in memory *1/ */
/* 	if (!init_elements()) { */
/* 		fprintf(logfile, "Could not load element information into memory.\n"); */
/* 		ok = 0; */
/* 	} */

/* 	/1* Start looping. *1/ */
/* 	while (ok) { */
/* 		/1* TODO: Read a line. *1/ */
/* 		/1* TODO: Tokenize the line: */
/* 		 * if ((token = sc_tokenize(line))) */
/* 		 * push token on stack */
/* 		 * do other things */
/* 		 * else */
/* 		 * notify user of error */
/* 		 * read the line again */
/* 		 *1/ */
/* 	} */

/* 	/1* Exit. *1/ */
/* 	if (stack) */
/* 		delete_stack(stack); */
/* 	end_elements(); */
/* } */

