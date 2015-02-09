#include "constants.h"

long double const_search(const char *line)
{
	FILE *const_file;
	char *read_const = NULL;
	size_t read_const_len = 0;
	long double constant = 0;
	int found_constant = 0;

	if ((const_file = fopen(CONST_FILE, "r"))) {
		while (getline(&read_const, &read_const_len, const_file) != -1
				&& !found_constant) {

			if (strncmp(read_const, line, strlen(line)) == 0) {
				found_constant = 1;

				free(read_const);
				read_const = NULL;
				read_const_len = 0;

				getline(&read_const, &read_const_len, const_file);
				constant = strtold(read_const, NULL);
			}

			free(read_const);
			read_const = NULL;
			read_const_len = 0;
		}
	}

	fclose(const_file);
	return constant;
}

int test_constants(WINDOW *outwin)
{
	char *mol = "mol";
	long double expected = 6.022141e+23;
	long double found;
	int correct;

	found = const_search(mol);
	if (found != expected)
		correct = 1;
	else
		correct = 0;

	wprintw(outwin,
			"According to test_constants(), %s is %Le (%s, should be %Le).\n",
			mol, found, ((correct) ? "Correct" : "Incorrect"), expected);

	return correct;
}

