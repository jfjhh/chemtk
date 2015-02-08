#include "pager.h"

int page_line(WINDOW *outwin, FILE *file)
{
	int status;
	char *line = NULL;
	size_t bytes = 0;

	if (getline(&line, &bytes, file) == -1) {
		status = 0;
	} else {
		wprintw(outwin, "%s", line);
		status = 1;
	}

	free(line);
	return status;
}

