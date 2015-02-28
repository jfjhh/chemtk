#include "num_str.h"

int sig_figs(const char *line)
{
	int i, start, figs, lastzeros;

	/* Check for unlimited sig. figs. (constant). */
	if (line[NUMSTR_BUFSIZE - 2] == NUMSTR_CONST_CHAR)
		return -1;

	/* Go past padding zeros and store that position in start. */
	for (i = 0; line[i] != '\0'
			&& !(line[i] != '0' && isdigit(line[i])); i++)
		;
	start = i;

	/* DEBUG */
	fprintf(stderr, "%s\n", line);
	for (figs = 1; figs <= start; figs++)
		fputc(' ', stderr);
	fprintf(stderr, "^\n\n");
	/* END DEBUG */

	/* Go either to end or decimal point. */
	for (i = start; line[i] != '\0' && line[i] != '.'; i++)
		;

	/* Check for decimal and calculate sig. figs. accordingly. */
	figs = 0;
	if (line[i] == '.') { /* Zeros are significant. */

		/* At this point, it is certain that everything left is a sig. fig. */
		for (i = start; line[i] != '\0'
				&& line[i] != 'e' && line[i] != 'E'; i++)
			if (isdigit(line[i]))
				figs++;

	} else { /* Zeros are non-significant. */
		lastzeros = 0;

		/* Count all digits until the end. */
		for (i = start; line[i] != '\0'
				&& line[i] != 'e' && line[i] != 'E'; i++) {
			if (line[i] == '0') {
				lastzeros++;
			} else if (isdigit(line[i])) {
				if (lastzeros > 0) {
					figs += lastzeros;
					lastzeros = 0;
				}
				figs++;
			}
		}
	}

	return figs;
}

int sig_before(const char *line)
{
	int i, start, figs, lastzeros;

	/* Check for unlimited sig. figs. (constant). */
	if (line[NUMSTR_BUFSIZE - 2] == NUMSTR_CONST_CHAR)
		return -1;

	/* Go past padding zeros and store that position in start. */
	for (i = 0; line[i] != '\0'
			&& !(line[i] != '0' && isdigit(line[i])); i++)
		;
	start = i;

	/* DEBUG */
	fprintf(stderr, "%s\n", line);
	for (figs = 1; figs <= start; figs++)
		fputc(' ', stderr);
	fprintf(stderr, "^\n\n");
	/* END DEBUG */

	/* Go either to end or decimal point. */
	for (i = start; line[i] != '\0' && line[i] != '.'; i++)
		;

	/* Check for decimal and calculate sig. figs. accordingly. */
	figs = 0;
	if (line[i] == '.') { /* Zeros are significant. */

		/* At this point, it is certain that everything left before
		 * the decimal is a sig. fig. */
		for (i = start; line[i] != '\0'; i++)
			if (isdigit(line[i]))
				figs++;

	} else { /* Zeros are non-significant. */
		lastzeros = 0;

		/* Count all digits until the end. */
		for (i = start; line[i] != '\0'; i++) {
			if (line[i] == '0') {
				lastzeros++;
			} else if (isdigit(line[i])) {
				if (lastzeros > 0) {
					figs += lastzeros;
					lastzeros = 0;
				}
				figs++;
			}
		}
	}

	return figs;
}

int sig_after(const char *line)
{
	int i, start, figs, lastzeros;

	/* Check for unlimited sig. figs. (constant). */
	if (line[NUMSTR_BUFSIZE - 2] == NUMSTR_CONST_CHAR)
		return -1;

	/* Go to after decimal (if any). */
	for (i = 0; line[i] != '\0' && line[i] != '.'; i++)
		;

	if (line[i] == '\0') /* No decimal. */
		start = 0;
	else /* Decimal. */
		start = i + 1;

	/* DEBUG */
	fprintf(stderr, "%s\n", line);
	for (figs = 1; figs <= start; figs++)
		fputc(' ', stderr);
	fprintf(stderr, "^\n\n");
	/* END DEBUG */

	/* Check for decimal and calculate sig. figs. accordingly. */
	figs = 0;
	if (start) { /* Zeros are significant. */

		/* At this point, it is certain that everything left is a sig. fig. */
		for (i = start; line[i] != '\0'; i++)
			if (isdigit(line[i]))
				figs++;

	} else { /* Zeros are non-significant. */
		lastzeros = 0;

		/* Count all digits until the end. */
		for (i = start; line[i] != '\0'; i++) {
			if (line[i] == '0') {
				lastzeros++;
			} else if (isdigit(line[i])) {
				if (lastzeros > 0) {
					figs += lastzeros;
					lastzeros = 0;
				}
				figs++;
			}
		}
	}

	return figs;
}

struct num_str *get_num_str(const char *line)
{
	int i, is_num;
	long double tmp;
	struct num_str *read;

	if (!(read = (struct num_str *) malloc(sizeof(struct num_str))))
		return NULL;

	is_num = 0;
	read->type = NONE;
	for (i = 0; i < 10; i++) {
		if (line[0] == '0' + i) {
			sscanf(line, "%Le", &(read->data));

			if (line[NUMSTR_BUFSIZE - 2] == NUMSTR_CONST_CHAR)
				read->type = CONSTANT;
			else
				read->type = NUMBER;

			read->sig_figs = sig_figs(line);
			is_num = 1;

			break;
		}
	}

	if (!is_num) {
		if ((tmp = const_search(line)) != 0) {
			read->type = CONSTANT;
		} else {
			read->type = CMD;
		}
		read->data = tmp;
	}

	strncpy(read->str, line, NUMSTR_BUFSIZE);
	read->str[NUMSTR_BUFSIZE - 1] = '\0'; /* Force NULL termination. */

	return read;
}

/* TODO: write test_num_str(). */
int test_num_str(WINDOW *outwin)
{
	int i, status, result_figs;
	const int correct_figs[NUMSTR_FIG_TESTS] = {
		3, 5, 2, 4,
	};
	const char *test_figs[NUMSTR_FIG_TESTS] = {
		"00.128",
		"23.020",
		"34000",
		"34010",
	};
	i = status = 0;

	wprintw(outwin, "TODO: write test_num_str().\n");

	wprintw(outwin, " *\tTesting sig_figs() on %d numbers.\n", NUMSTR_FIG_TESTS);

	for (i = 0; i < NUMSTR_FIG_TESTS; i++) {
		result_figs = sig_figs(test_figs[i]);

		wprintw(outwin,
				"'%s' has %d sig. figs "
				"(should be %d: %d before and %d after).\n",
				test_figs[i], result_figs,
				correct_figs[i],
				sig_before(test_figs[i]),
				sig_after(test_figs[i]));

		status += ((correct_figs[i] == result_figs) ? 1 : 0);
	}
	wprintw(outwin, " *\tGot %d / %d sig. fig. calculations right.\n",
			status, NUMSTR_FIG_TESTS);

	if (status == NUMSTR_FIG_TESTS)
		return 1;
	else
		return 0;
}

