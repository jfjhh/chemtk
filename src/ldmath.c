/**
 * @file ldmath.c
 * @author Alex Striff
 *
 * @brief Provides routines for simple number comparisons and significant figure
 * calculations.
 *
 * @note A <tt>significant figure array</tt> is defined as an array of @c ints
 * that is @c SIG_FIG_LEN long, that holds significant figure data on a number.
 */

#include "ldmath.h"

int valid_num(const char *line)
{
	double d;

	d = strtod(line, NULL);

	if (d == 0F || d == HUGE_VALF)
		return 0;
	else
		return 1;
}

int sig_figs(const char *line, int *figs)
{
	int i, start, lastzeros, tmp;

	SIG_FIG_SET(figs, 0);

	/* Check if line is valid */
	if (!valid_num(line))
		return 0;

	/* Check for unlimited sig. figs. (constant). */
	if (line[SC_TOKEN_INLEN - 2] == SC_CONST_CHAR) {
		SIG_FIG_SET(figs, -1);
		return -1;
	}

	/* Go past padding zeros and store that position in start. */
	for (i = 0; line[i] != '\0'
			&& !(line[i] != '0' && isdigit(line[i])); i++)
		;
	start = i;

	/* Go either to end or decimal point. */
	for (i = start; line[i] != '\0' && line[i] != '.'; i++)
		;

	/* Save */
	figs[ALL] = figs[BEFORE] = i;

	/* Check for decimal and calculate sig. figs. accordingly. */
	tmp = 0;
	if (line[i] == '.') { /* Zeros are significant. */

		/* At this point, it is certain that everything left is a sig. fig. */
		for (i = start; line[i] != '\0'
				&& line[i] != 'e' && line[i] != 'E'; i++)
			if (isdigit(line[i]))
				tmp++;

	} else { /* Zeros are non-significant. */
		lastzeros = 0;

		/* Count all digits until the end. */
		for (i = start; line[i] != '\0'
				&& line[i] != 'e' && line[i] != 'E'; i++) {
			if (line[i] == '0') {
				lastzeros++;
			} else if (isdigit(line[i])) {
				if (lastzeros > 0) {
					tmp      += lastzeros;
					lastzeros =  0;
				}
				tmp++;
			}
		}
	}

	figs[ALL]   += tmp;
	figs[AFTER] =  tmp;

	return figs[ALL];
}

int test_ldmath(FILE *logfile)
{
	int i, j,  status, result_figs[SIG_FIG_LEN];
	const int correct_figs[SFC_FIG_TESTS][SIG_FIG_LEN] = {
		{ 3, 0, 3 },
		{ 6, 2, 3 },
		{ 2, 2, 0 },
		{ 4, 4, 0 }
	};
	const char *test_figs[SFC_FIG_TESTS] = {
		"00.128",
		"23.020",
		"34000",
		"34010",
	};
	i = j = status = 0;

	fprintf(logfile, " *\tTesting sig_figs() on %d numbers.\n", SFC_FIG_TESTS);

	for (i = 0; i < SFC_FIG_TESTS; i++) {
		sig_figs(test_figs[i], result_figs);

		for (j = ALL; j <= AFTER; j++) {
			fprintf(logfile,
					"'%s' has %d sig. figs "
					"(should be %d).\n",
					test_figs[i], result_figs[j],
					correct_figs[i][j]);

			/**
			 * @test Checks if significant figure fields can be correctly
			 * calculated. More specifically, it checks if all of
			 * @c SFC_FIG_TESTS numbers have all of
			 * @c SIG_FIG_LEN significant figure fields calculated correctly by
			 * @c sig_figs().
			 */
			status += ((correct_figs[i][j] == result_figs[j]) ? 1 : 0);
		}
	}

	/*
	 * SFC_FIG_TESTS * SIG_FIG_LEN
	 * Total number of sig. fig. fields tested.
	 */
	fprintf(logfile, " *\tGot %d / %d sig. fig. calculations right.\n",
			status, SFC_FIG_TESTS * SIG_FIG_LEN);

	if (status == SFC_FIG_TESTS * SIG_FIG_LEN)
		return 1;
	else
		return 0;
}

