#include "ldmath.h"

int sig_figs(const char *line)
{
	int i, start, lastzeros, figs;

	/* Check for unlimited sig. figs. (constant). */
	if (line[SC_TOKEN_INLEN - 2] == SC_CONST_CHAR)
		return -1;

	/* Go past padding zeros and store that position in start. */
	for (i = 0; line[i] != '\0'
			&& !(line[i] != '0' && isdigit(line[i])); i++)
		;
	start = i;

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
					figs      += lastzeros;
					lastzeros =  0;
				}
				figs++;
			}
		}
	}

	return figs;
}

int test_ldmath(FILE *logfile)
{
	int i, status, result_figs;
	const int correct_figs[SFC_FIG_TESTS] = {
		3, 5, 2, 4,
	};
	const char *test_figs[SFC_FIG_TESTS] = {
		"00.128",
		"23.020",
		"34000",
		"34010",
	};
	i = status = 0;

	fprintf(logfile, " *\tTesting sig_figs() on %d numbers.\n", SFC_FIG_TESTS);

	for (i = 0; i < SFC_FIG_TESTS; i++) {
		result_figs = sig_figs(test_figs[i]);

		fprintf(logfile,
				"'%s' has %d sig. figs "
				"(should be %d).\n",
				test_figs[i], result_figs,
				correct_figs[i]);

		status += ((correct_figs[i] == result_figs) ? 1 : 0);
	}
	fprintf(logfile, " *\tGot %d / %d sig. fig. calculations right.\n",
			status, SFC_FIG_TESTS);

	if (status == SFC_FIG_TESTS)
		return 1;
	else
		return 0;
}

