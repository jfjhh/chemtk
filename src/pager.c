#include "pager.h"

static int page_line(WINDOW *outwin, FILE *file)
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

void page_prompt(WINDOW *outwin, const char *prompt)
{
	wattron(outwin, A_BOLD);
	wprintw(outwin, "<Press any key to %s>", prompt);
	wattroff(outwin, A_BOLD);
	wgetch(outwin);
}

void page_file(WINDOW *outwin, FILE *file)
{
	wattroff(outwin, A_BOLD);
	while (page_line(outwin, file)) {
		if (getcury(outwin) >= getmaxy(outwin) - 1) {
			page_prompt_file(outwin);
			werase(outwin);
			wmove(outwin, 0, 0);
		}
	}
}

int test_pager(WINDOW *outwin)
{
	FILE *test_file = fopen(TEST_PAGER_FILE, "r");
	page_file(outwin, test_file);
	fclose(test_file);

	return 1;
}

