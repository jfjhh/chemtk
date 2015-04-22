#include "chemtk.h"

int main(void)
{
	FILE *scicalc_doc = NULL;
	WINDOW *devwin, *devwinborder, *sciwin, *sciwinborder,
		   *infowin, *infowinborder;
	devwin = devwinborder = sciwinborder = sciwin =
		infowin = infowinborder = NULL;

	initscr();
	start_color();
	keypad(stdscr, TRUE);
	cbreak();
	noecho();

	/* Init sciwin. */
	sciwinborder = newwin((LINES / 2),      (COLS / 2),      0,  (COLS / 2));
	sciwin       = newwin((LINES / 2) - 2,  (COLS / 2) - 2,  1,  (COLS / 2) + 1);
	keypad(sciwin, TRUE);
	box(sciwinborder, 0, 0);
	wrefresh(sciwinborder);

	keypad(sciwin, TRUE);
	init_pair(SCIWIN_COLOR_NORMAL,  COLOR_BLUE,   COLOR_BLACK);
	init_pair(SCIWIN_COLOR_PROMPT,  COLOR_GREEN,  COLOR_BLACK);
	wattrset(sciwin, A_BOLD | COLOR_PAIR(SCIWIN_COLOR_NORMAL));

	/* Init infowin. */
	infowinborder = newwin((LINES / 2),      (COLS / 2),
			(LINES / 2),                     (COLS / 2));

	infowin       = newwin((LINES / 2) - 2,  (COLS / 2) - 2,
			(LINES / 2) + 1,                 (COLS / 2) + 1);

	keypad(infowin, TRUE);
	box(infowinborder, 0, 0);
	wrefresh(infowinborder);

	keypad(infowin, TRUE);
	init_pair(INFOWIN_COLOR_NORMAL, COLOR_MAGENTA, COLOR_BLACK);
	wattrset(infowin, A_BOLD | COLOR_PAIR(INFOWIN_COLOR_NORMAL));

	/* Init devwin. */
	devwinborder = newwin(LINES,  (COLS / 2),      0,  0);
	devwin = newwin(LINES-2,      (COLS / 2) - 2,  1,  1);
	box(devwinborder, 0, 0);
	wrefresh(devwinborder);

	/* Run init tests in devwin */
	init_pair(STACK_COLOR_NORMAL, COLOR_RED, COLOR_BLACK);
	wattrset(devwin, A_BOLD | COLOR_PAIR(STACK_COLOR_NORMAL));

	if (run_all_tests(devwin) < NUM_TESTS) {
		page_prompt(devwin, "exit (cannot continue if some tests failed)");
		goto exit;
	}

	/* Also display scicalc docfile in devwin. */
	scicalc_doc = fopen(SCICALC_DOCFILE, "r");

	werase(devwin);
	if (scicalc_doc == NULL) {
		wprintw(devwin, "Oops! Bad fopen()!\n");
	} else {
		page_file(devwin, scicalc_doc);
	}

	/* Also print a ptable. */
	print_ptable_centered(devwin, getcury(devwin) - 1);
	wprintw(devwin, "\n");
	wmove(devwin, getcury(devwin) - 1, 0);
	wrefresh(devwin);

	/* Scicalc window */
	scicalc(sciwin, infowin);

exit: /* Exit */
	if (scicalc_doc)
		fclose(scicalc_doc);

	delwin(devwin);
	delwin(devwinborder);
	delwin(sciwin);
	delwin(sciwinborder);
	delwin(infowin);
	delwin(infowinborder);

	endwin();
	return 0;
}

