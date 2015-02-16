#include "chemtk.h"

int main(void)
{
	FILE *scicalc_doc;
	WINDOW *devwin, *devwinborder, *sciwin, *sciwinborder;
	devwin = devwinborder = sciwinborder = sciwin = NULL;

	initscr();
	start_color();
	keypad(stdscr, TRUE);
	cbreak();
	noecho();

	/* Init sciwin. */
	sciwinborder = newwin((LINES / 2), (COLS / 2), 0, (COLS / 2));
	sciwin = newwin((LINES / 2)-2, (COLS / 2)-2, 1, (COLS / 2)+1);
	box(sciwinborder, 0, 0);
	wrefresh(sciwinborder);

	init_pair(SCIWIN_COLOR_NORMAL, COLOR_BLUE, COLOR_BLACK);
	wattrset(sciwin, A_BOLD | COLOR_PAIR(SCIWIN_COLOR_NORMAL));

	/* Init devwin. */
	devwinborder = newwin(LINES, (COLS / 2), 0, 0);
	devwin = newwin(LINES-2, (COLS / 2)-2, 1, 1);
	box(devwinborder, 0, 0);
	wrefresh(devwinborder);

	/* Run init tests in devwin */
	init_pair(STACK_COLOR_NORMAL, COLOR_RED, COLOR_BLACK);
	wattrset(devwin, A_BOLD | COLOR_PAIR(STACK_COLOR_NORMAL));

	page_prompt(devwin, "run tests");
	werase(devwin);
	run_all_tests(devwin);

	/* Also display scicalc docfile in devwin. */
	page_prompt(devwin, "see scicalc doc file");
	werase(devwin);

	scicalc_doc = fopen(SCICALC_DOCFILE, "r");

	if (scicalc_doc == NULL) {
		wprintw(devwin, "Oops! Bad fopen()!\n");
	} else {
		page_file(devwin, scicalc_doc);
	}

	/* Also print a ptable. */
	print_ptable(devwin, getcury(devwin) - 1, getcurx(devwin) + 3);
	wprintw(devwin, "\n");
	page_prompt(devwin, "continue");
	wmove(devwin, getcury(devwin)-1, 0);
	wclrtobot(devwin);
	wrefresh(devwin);

	/* Scicalc window */
	wmove(sciwin, 3, (getmaxx(sciwin) / 2) - 8);

	/* DEBUG */
	/* scicalc(sciwin); */
	page_prompt(sciwin, "exit");

	/* Exit */
	if (scicalc_doc)
		fclose(scicalc_doc);
	delwin(devwin);
	delwin(devwinborder);
	endwin();
	return 0;
}

