#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define max(A, B) ((A > B) ? A : B)

#define TITLE "\033[0;34m"
#define BANNER "\033[0;1;34m"
#define FIRST "\033[0;34m1> "
#define SECOND "\033[0;1;34m2> "
#define OPERATOR "\033[0;34mO> "
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

enum out_format { SCIENTIFIC, EXPANDED };

int get_dec(const char *line);
int calculate(long double *n);
long operate(long a, long b, char operation);

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
				printf("%s=> %Le%s\n\n", ANSWER, n, CLEAR);
			else
				printf("%s=> %Lf%s\n\n", ANSWER, n, CLEAR);
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

int calculate(long double *n)
{
	int digits_x, digits_y;
	long double x, y;
	long a, b, c;
	char op, *line;

	line = NULL;
	digits_x = digits_y = 0;
	a = b = c = 0L;
	x = y = 0;

	if ((line = readline(FIRST))) {
		if (line[0] == 'q') {
			free(line);
			return 0;
		} else {
			sscanf(line, "%Lg", &x);
			digits_x = get_dec(line);
			free(line);
		}
	} else {
		fprintf(stderr, "Could not read a line!\n");
		return 0;
	}

	if ((line = readline(SECOND))) {
		if (line[0] == 'q') {
			free(line);
			return 0;
		} else {
			sscanf(line, "%Lg", &y);
			digits_y = get_dec(line);
			free(line);
		}
	} else {
		fprintf(stderr, "Could not read a line!\n");
		return 0;
	}

	if ((line = readline(OPERATOR))) {
		if (line[0] == 'q') {
			free(line);
			return 0;
		} else {
			sscanf(line, "%c", &op);
			free(line);
		}
	} else {
		fprintf(stderr, "Could not read a line!\n");
		return 0;
	}

	a = x * max(digits_x, digits_y);
	b = y * max(digits_x, digits_y);

	c = operate(a, b, op);

	*n = ((long double) c / (long double) max(digits_x, digits_y));
	return 1;
}

long operate(long a, long b, char operation)
{
	long c;

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

