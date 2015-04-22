#include "pager.h"

#define page_line(A, B) page_line_offset(A, B, 0)
static int page_line_offset(WINDOW *outwin, FILE *file, int x_offset)
{
	int status;
	char *line   = NULL;
	size_t bytes = 0;

	if (getline(&line, &bytes, file) == -1) {
		status = 0;
	} else {
		mvwprintw(outwin, getcury(outwin), x_offset, "%s", line);
		status = 1;
	}

	free(line);
	return status;
}

void page_prompt(WINDOW *outwin, const char *prompt)
{
	wattron(outwin,   A_BOLD);
	wprintw(outwin,   "<Press any key to %s>",  prompt);
	wattroff(outwin,  A_BOLD);
	wgetch(outwin);
}

void page_bottom(WINDOW *outwin)
{
	if (getcury(outwin) >= getmaxy(outwin) - 1)
		werase(outwin);
}

void page_prompt_bottom(WINDOW *outwin, const char *prompt)
{ if (getcury(outwin) >= getmaxy(outwin) - 1) {
	wattron(outwin,   A_BOLD);
	wprintw(outwin,   "<Press any key to %s>",  prompt);
	wattroff(outwin,  A_BOLD);
	wgetch(outwin);
	werase(outwin);
											  }
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

void page_file_coords(WINDOW *outwin, FILE *file, int y, int x)
{
	wattroff(outwin, A_BOLD);
	wmove(outwin, y, x);

	while (page_line_offset(outwin, file, x)) {
		if (getcury(outwin) >= getmaxy(outwin) - 1) {
			page_prompt_file(outwin);
			werase(outwin);
			wmove(outwin, 0, 0);
		}
	}
}

void page_file_delay(WINDOW *outwin, FILE *file, useconds_t time)
{
	wattroff(outwin, A_BOLD);
	while (page_line(outwin, file)) {
		if (getcury(outwin) >= getmaxy(outwin) - 1) {
			usleep(time);
			werase(outwin);
			wmove(outwin, 0, 0);
		}
	}
}

int test_pager(WINDOW *outwin)
{
	FILE *test_file;

	if ((test_file = fopen(TEST_PAGER_FILE, "r"))) {
		page_file(outwin, test_file);
		fclose(test_file);
		return 1;
	} else {
		wprintw(outwin, "Cannot open test file for pager.\n");
		return 0;
	}
}

