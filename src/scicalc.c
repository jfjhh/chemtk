#include "scicalc.h"

const char *title = "Scicalc: A scientific calculator utility.";

#define NUM_OPERATORS 16
const char operators[] = {
	'+', '-', '*', '/',
	'=', '<', '>', '&',
	'^', '~', '%', '|',
	'o', 'l', 'r', 'x',
};

/* TODO: Actually calculate sig. figs. */
int get_sig(const char *line)
{
	int i, j;

	for (i = 0; line[i] != '\0' && line[i] != '.'; i++)
		;

	for (j = i; line[j] != '\0' && isdigit(line[j]); j++)
		;

	return j;
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

/* TODO: write test_scicalc(). */
int test_scicalc(WINDOW *outwin)
{
	wprintw(outwin, "TODO: write test_scicalc().\n");
	return 1;
}

