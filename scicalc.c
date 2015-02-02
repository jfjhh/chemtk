#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/* Strange floating point values. */
#define _GNU_SOURCE
#ifndef INFINITY /* ISO C99 standard. */
#define INFINITY 1.0 / 0.0
#endif
#ifndef NINFINITY /* ISO C99 standard. */
#define NINFINITY 0 - INFINITY
#endif
#ifndef N0 /* IEEE 754 */
#define N0 1.0 / -INFINITY
#endif
#ifndef NAN /* IEEE floating point. */
#define NAN -INFINITY
#endif

#define max(A, B) ((A > B) ? A : B)

#define TITLE "\033[0;34m"
#define BANNER "\033[0;1;34m"
#define FIRST "\033[0;1;34m1> "
#define SECOND "\033[0;1;35m2> "
#define OPERATOR "\033[0;1;36mO> "
#define ANSWER "\033[0;31m=> "
#define EXIT "\033[0;1;31m"
#define CLEAR "\033[0m"

#define NUM_CONSTANTS 5
const char *constants[NUM_CONSTANTS][2] = {
	{"mol", "6.02214129e+23"},
	{"pi", "3.1415926535897932"},
	{"l", "22.4"},
	{"c", "2.998792458e+8"},
	{"h", "6.6262e-34"},
};

#define NUM_OPERATORS 16
const char operators[] = {
	'+', '-', '*', '/',
	'=', '<', '>', '&',
	'^', '~', '%', '|',
	'o', 'l', 'r', 'x',
};

enum out_format { SCIENTIFIC, EXPANDED };

int get_dec(const char *line);
int getdouble(const char *prompt, long double *num);
int calculate(long double *n);
long double operate(long double a, long double b, char operation);

int main(int argc, const char **argv)
{
	long double n = 0L;
	int quit = 0;
	enum out_format format;

	if ((argc == 2) && (strncmp(argv[1], "-n", 2)) == 0)
		format = EXPANDED;
	else
		format = SCIENTIFIC;

	/* Banner / Usage Info. */
	fprintf(stderr,
			"%s"
			"\n\t\tScicalc: A scientific calculator utility.\n\n"
			"%s"
			"\tEnter two numbers, and then an operation.\n"
			"\tOps: + - * / = < > & ^ ~ %% | o l r x.\n"
			"\tScientific notation is written '1.2e3' '1.2e+3' or '1.2e-3'.\n"
			"\tJust pressing <enter> uses the last result.\n"
			"\tSignificant figures are automatically calculated.\n"
			"\tType a contant (unlimited sig. figs.) by it's name ...\n"
			"\t(e.g. mol, pi), or by appending a '.' to the number ...\n"
			"\t(e.g. 22.4. or 2.99e8.).\n"
			"\tType 'q' to quit.\n\n"
			"%s",
			TITLE, BANNER, CLEAR);

	while (!quit)
		if (calculate(&n))
			if (format == SCIENTIFIC)
				printf("%s%Le%s\n\n", ANSWER, n, CLEAR);
			else
				printf("%s%Lf%s\n\n", ANSWER, n, CLEAR);
		else
			quit = 1;

	fprintf(stderr, "\n%sExiting ...%s\n", EXIT, CLEAR);
	return 0;
}

int get_dec(const char *line)
{
	int i, j;

	for (i = 0; line[i] != '\0' && line[i] != '.'; i++)
		;

	for (j = i; line[j] != '\0' && isdigit(line[j]); j++)
		;

	return j;
}

int getdouble(const char *prompt, long double *num)
{
	char *line = NULL;
	int i, read_constant, ok;
	ok = read_constant = 0;

	while (!ok) {
		if ((line = readline(prompt))) {
			if (line[0] == 'q') {
				free(line);
				return 0;
			}

			for (i = 0; i < NUM_CONSTANTS; i++)
				if (!strncmp(constants[i][0], line, strlen(constants[i][0]) + 1))
					*num = strtod(constants[i][1], NULL);

			if (!read_constant) {
				if (line[0] == '\0') {
					ok = 2; /* arbitrary code for 'use previous number' */
				} else if (!sscanf(line, "%Lg", num)) {
					fprintf(stderr, "Error! Try again.\n");
				} else {
					ok = 1;
				}
			}

			free(line);
		} else {
			fprintf(stderr, "Could not read a line!\n");
			return 0;
		}
	}

	return ok;
}

int calculate(long double *n)
{
	int i, ok;
	long double x, y;
	char op, *line;
	ok = i = 0;
	op = '+'; /* default, should be overrided. */
	x = y = 0;

	if ((ok = getdouble(FIRST, &x))) {
		if (ok == 2)
			x = *n;
	} else {
		return 0;
	}

	if ((ok = getdouble(SECOND, &y))) {
		if (ok == 2)
			y = *n;
	} else {
		return 0;
	}

	ok = 0;
	while (!ok) {
		if ((line = readline(OPERATOR)) && (sscanf(line, "%c", &op))) {
			if (op == 'q') {
				free(line);
				return 0;
			}

			for (i = 0; i < NUM_OPERATORS; i++)
				if (operators[i] == op)
					ok = 1;

			if (!ok)
				fprintf(stderr, "Error! Please retry.\n");

			free(line);
		}
	}

	*n = operate(x, y, op);
	return 1;
}

long double operate(long double a, long double b, char operation)
{
	long double c;

	switch (operation) {
		case '+':
			c = a + b;
			break;
		case '-':
			c = a - b;
			break;
		case '*':
			c = a * b;
			break;
		case '/':
			c = a / b;
			break;
		case '%':
			c = (int) a % (int) b;
			break;
		case '&':
			c = (int) a & (int) b;
			break;
		case '|':
			c = (int) a | (int) b;
			break;
		case 'x':
			c = (int) a ^ (int) b;
			break;
		case 'l':
			c = (int) a << (int) b;
			break;
		case 'r':
			c = (int) a >> (int) b;
			break;
		case '~':
			c = ~ ((int) a);
			break;
		case '>':
			c = (a > b) ? 1L : 0L;
			break;
		case '<':
			c = (a < b) ? 1L : 0L;
			break;
		case '=':
			c = (a == b) ? 1L : 0L;
			break;
		case '^':
			c = pow(a, b);
			break;
		case 'o':
			c = (logl(a) / logl(b));
			break;
		default:
			c = 0L;
			break;
	}

	return c;
}

