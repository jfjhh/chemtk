#include "constant.h"

double const_search(const char *line)
{
	FILE *const_file;
	char *read_const      = NULL;
	size_t read_const_len = 0;
	double constant       = 0.0;
	int found_constant    = 0;

	/* if ((const_file = fopen(CONST_FILE, "r"))) { */
	if (access(CONST_FILE, R_OK) == -1) {
		perror("const_search: not able to read '" CONST_FILE "'");
		return 0.0;
	}

	const_file = fopen(CONST_FILE, "r");
	while (getline(&read_const, &read_const_len, const_file) != -1
			&& !found_constant) {

		if (strncmp(read_const, line, strlen(line)) == 0) {
			free(read_const);

			found_constant    = 1;
			read_const        = NULL;
			read_const_len    = 0;

			getline(&read_const, &read_const_len, const_file);
			constant = strtod(read_const, NULL);
		}

		free(read_const);
		read_const     = NULL;
		read_const_len = 0;
	}

	fclose(const_file);
	free(read_const);

	return constant;
}

int test_consant(FILE *logfile)
{
	char *mol            = "mol";
	double expected = 6.022141e+23L;
	double found;
	int correct;

	found = const_search(mol);

	/* Check if they are "equal enough" (within DBL_EPSILON). */
	if (fabsl(expected - found) < DBL_EPSILON)
		correct = 1;
	else
		correct = 0;

	fprintf(logfile,
			"According to test_consant(), %s is %e (%s, should be %e).\n",
			mol, found, ((correct) ? "Correct" : "Incorrect"), expected);

	return correct;
}

