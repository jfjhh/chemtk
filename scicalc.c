/*
 * TODO: Do Sig. Fig. Rounding in calculations.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#define LINE_MIN 16

#define TITLE "\e[0;34m"
#define BANNER "\e[0;1;34m"
#define FIRST "\e[0;34m"
#define SECOND "\e[0;1;34m"
#define OPERATOR "\e[0;34m"
#define ANSWER "\e[0;31m"
#define EXIT "\e[0;1;31m"
#define CLEAR "\e[0m"

#define NUM_CONSTANTS 4
const char *constants[NUM_CONSTANTS][2] = {
	{"mol", "6.02214129e+23"},
	{"pi", "3.1415926535897932"},
	{"l", "22.4"},
	{"c", "2.998792458e+8"},
	{"h", "6.6262e-34"},
};

int getnum(long double *read, int *figs);
int promptnum(long double *n, const char *prompt, ...);
int get_figs(char *s);
int get_dec(char *s);
int calculate(long double *n);
long double operate(long double a, long double b, char operation, int figs);

int main(int argc, const char **argv)
{
	long double n = 0L;
	int quit = 0;

	int normal;
	if ((argc == 2) && (strncmp(argv[1], "-n", 2)) == 0)
		normal = 1;
	else
		normal = 0;

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
			if (normal)
				printf("%s=> %Lf%s\n\n", ANSWER, n, CLEAR);
			else
				printf("%s=> %Le%s\n\n", ANSWER, n, CLEAR);
		else
			quit = 1;

	fprintf(stderr, "\n%sExiting ...%s\n", EXIT, CLEAR);
	return 0;
}

long double operate(long double a, long double b, char operation, int figs)
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

int getnum(long double *read, int *figs)
{
	size_t linelen = LINE_MIN;
	char *line, empty, quit, const_delimiter;
	int ret, c, constant;
	ret = c = constant = 0;
	line = malloc(sizeof(char) * linelen);
	empty = '\n';
	quit = 'q';
	const_delimiter = '.';


	if (getline(&line, &linelen, stdin) != -1) {
		if (line[0] == empty) {
			/* fprintf(stderr, "In previous bit.\n"); */
			ret = 2; /* Use the previous result. */
		} else if (line[0] == quit) {
			/* fprintf(stderr, "In quit bit.\n"); */
			ret = 0; /* Quit. */
			*figs = 0;
		} else {
			/* fprintf(stderr, "In good bit.\n"); */
			for (c = 0; c <= NUM_CONSTANTS - 1; c++) {
				/* fprintf(stderr, "In constants bit.\n"); */
				if (strncmp(constants[c][0], line, strlen(constants[c][0])) == 0) {
					/* fprintf(stderr, "Matched: %s.\n", constants[c][0]); */
					*read = strtold(constants[c][1], NULL);
					constant = 1;
					break;
				}
			}

			if ((!constant) && (line[strlen(line) - 1] == const_delimiter)) {
				sscanf(line, "%Le", read);
				*figs = get_figs(line);
			} else {
				*figs = -1;
			}

		}
	} else {
		/* fprintf(stderr, "In error bit.\n"); */
		read = 0L;
		ret = 0; /* Error. */
	}

	free(line);
	return ret;
}

int get_dec(char *s)
{
	char c;
	int i, count, dec;
	i = count = dec = 0;

	while ((c = s[i]) != '\0') {

		if (c == '.')
			dec++;
		else if ((c >= '0' && c <= '9') && dec)
			count++;

		i++;
	}

	if (dec == 2)
		count = -1;

	return count;
}

int get_figs(char *s)
{
	char c;
	int i, figs, dec;
	i = figs = dec = 0;

	while ((c = s[i]) != '\0') {

		if (c == '0' && dec)
			figs++;
		else if (c >= '1' && c <= '9')
			figs++;
		else if (c == '0' && figs > 0)
			figs++;
		else if (c == '.')
			dec++;

		i++;
	}

	if (dec == 2)
		figs = -1;

	return figs;
}

int promptnum(long double *n, const char *prompt, ...)
{
	va_list format_args;
	int read_ok, sig_figs;
	sig_figs = 0;

	va_start(format_args, prompt);
	vprintf(prompt, format_args);
	va_end(format_args);

	read_ok = getnum(n, &sig_figs);
	printf("%s", CLEAR);

	if (!read_ok)
		return 0;
	else
		return sig_figs;
}

int calculate(long double *n)
{
	long double a, b;
	int read_ok, figs_a, figs_b, tmp, nondec;
	char operation, *line;
	size_t linelen = LINE_MIN;
	line = malloc(sizeof(char) * linelen);
	a = b = *n;

	/* Get first number. */
	figs_a = promptnum(n, "%s1> ", FIRST);
	if (!figs_a)
		goto exit;

	/* Get second number. */
	figs_b = promptnum(n, "%s2> ", SECOND);
	if (!figs_b)
		goto exit;

	/* Get the operation char. */
	printf("%so> ", OPERATOR);
	getline(&line, &linelen, stdin);
	read_ok = sscanf(line, "%c", &operation);
	printf("%s", CLEAR);
	if (!read_ok)
		goto exit;
	else if (operation == '+') { /* Find decimal places. */
		tmp = figs_a;
		for (nondec = 1; (tmp /= 10) < 0; nondec++)
			;
		figs_a = figs_a - nondec;

		tmp = figs_b;
		for (nondec = 1; (tmp /= 10) < 0; nondec++)
			;
		figs_b = figs_b - nondec;
	}

	*n = operate(a, b, operation, (figs_a < figs_b) ? a : b);

	free(line);
	return 1;

exit:
	free(line);
	return 0;
}

