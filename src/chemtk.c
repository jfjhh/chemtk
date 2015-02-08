#include <ncurses.h>

#include "stack.h"
#include "scicalc.h"
#include "pager.h"

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

	/* Check if at bottom of window, then clear. */
	/* if (getcury(devwin) >= getmaxy(devwin)-1) */
	/* 	werase(devwin); */

	wprintw(devwin, "Testing stack ...\n");
	wprintw(devwin, "\nTest %s : Abstract Data Type : Stack\n",
			(test_stack(devwin) ? "Succeeded" : "Failed"));

	wprintw(devwin, "\n<Press any key to see docfile>");
	wgetch(devwin);
	werase(devwin);

	scicalc_doc = fopen(SCICALC_DOCFILE, "r");
	if (scicalc_doc == NULL) {
		wprintw(devwin, "Oops! Bad fopen()!\n");
	} else {
		wattroff(devwin, A_BOLD);
		while (page_line(devwin, scicalc_doc)) {
			if (getcury(devwin) >= getmaxy(devwin) - 1) {
				wattron(devwin, A_BOLD);
				wprintw(devwin, "<Press any key to see next page>");
				wattroff(devwin, A_BOLD);
				wgetch(devwin);
				werase(devwin);
				wmove(devwin, 0, 0);
			}
		}
	}

	wattron(devwin, A_BOLD);
	wprintw(devwin, "<Press any key to continue>");
	wgetch(devwin);
	werase(devwin);
	wattroff(devwin, A_BOLD);

	/* Scicalc */
	wmove(sciwin, 3, (getmaxx(sciwin) / 2) - 8);
	scicalc(sciwin);

	/* Exit */
	if (scicalc_doc)
		fclose(scicalc_doc);
	delwin(devwin);
	delwin(devwinborder);
	endwin();
	return 0;
}

