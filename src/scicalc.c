#include "scicalc.h"

const char *title = "Scicalc: A scientific calculator utility.";

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

int scicalc(WINDOW *outwin)
{
	long double n = 0L;
	int quit = 0;
	enum out_format format;

	/*
	   if ((argc == 2) && (strncmp(argv[1], "-n", 2)) == 0)
	   format = EXPANDED;
	   else
	   format = SCIENTIFIC;
	   */
	format = SCIENTIFIC;

	/* Banner / Usage Info. */
	mvwprintw(outwin, 0, getmaxx(outwin) / 2 - (strlen(title) / 2), "%s", title);
	wgetch(outwin);

	/* Begin the main calculation. */
	while (!quit) {
		wrefresh(outwin);
		if (calculate(outwin, &n))
			if (format == SCIENTIFIC)
				wprintw(outwin, "%Le\n\n", n);
			else
				wprintw(outwin, "%Lf\n\n", n);
		else
			quit = 1;
	}

	wprintw(outwin, "Exiting.\n");

	endwin();
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

int getdouble(WINDOW *outwin, const char *prompt, long double *num)
{
	char line[LINE_LEN] = {0};
	int i, read_constant, ok;
	ok = read_constant = 0;

	while (!ok) {
		wprintw(outwin, "%s", prompt);
		wgetch(outwin); /* DEBUG */
		if (getnstr(line, LINE_LEN) == OK) {
			if (line[0] == 'q') {
				return 0;
			}

			for (i = 0; i < NUM_CONSTANTS; i++)
				if (!strncmp(constants[i][0], line, strlen(constants[i][0]) + 1))
					*num = strtod(constants[i][1], NULL);

			if (!read_constant) {
				if (line[0] == '\0') {
					ok = 2; /* arbitrary code for 'use previous number' */
				} else if (!sscanf(line, "%Lg", num)) {
					wprintw(outwin, "Error! Try again.\n");
				} else {
					ok = 1;
				}
			}

		} else {
			wprintw(outwin, "Could not read a line!\n");
			return 0;
		}
	}

	return ok;
}

int calculate(WINDOW *outwin, long double *n)
{
	int i, ok;
	long double x, y;
	char op;
	ok = i = 0;
	op = '+'; /* default, should be overrided. */
	x = y = 0;

	if ((ok = getdouble(outwin, "1> ", &x))) {
		if (ok == 2)
			x = *n;
	} else {
		return 0;
	}

	if ((ok = getdouble(outwin, "2> ", &y))) {
		if (ok == 2)
			y = *n;
	} else {
		return 0;
	}

	ok = 0;
	while (!ok) {
		wprintw(outwin, "o> ");
		op = getch();
		if (op == 'q') {
			return 0;
		}

		for (i = 0; i < NUM_OPERATORS; i++)
			if (operators[i] == op)
				ok = 1;

		if (!ok) {
			attron(COLOR_PAIR(2) | A_BOLD);
			wprintw(outwin, "Error! Please retry.\n");
			attroff(COLOR_PAIR(2) | A_BOLD);
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

