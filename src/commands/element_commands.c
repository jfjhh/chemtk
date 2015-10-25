#include "element_commands.h"

	__attribute__((unused))
int element_information(char *line,
		__attribute__((unused)) sc_token *out,
		__attribute__((unused)) FILE *logfile)
{
	return print_element(find_element_sym(line), logfile);
}

	__attribute__((unused))
int element_verbose_information(char *line,
		__attribute__((unused)) sc_token *out,
		__attribute__((unused)) FILE *logfile)
{
	return print_element_info(find_element_sym(line), logfile);
}

	__attribute__((unused))
int element_periodic_table(__attribute__((unused)) char *line,
		__attribute__((unused)) sc_token *out,
		FILE *logfile)
{
	return print_ptable(logfile);
}

