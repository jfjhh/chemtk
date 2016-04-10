/**
 * @file cli.h
 * @author Alex Striff
 *
 * @brief Main file for ChemTK NCurses CLI.
 */

#include "cli.h"

static SCREEN *scr;
static WINDOW *dbgwin, *infowin, *calcwin;
static long dbgpos;

static void cwprintw(WINDOW *win, const char *s, ...)
{
	va_list args;
	va_start(args, s);
	wmove(win, getcury(win), (getmaxx(win) - strlen(s)) / 2);
	vw_printw(win, s, args);
	wmove(win, getcury(win) + 1, 0);
}

static void update_bg(FILE *logfile)
{
	fseek(logfile, dbgpos, SEEK_SET);

	int c;
	while ((c = fgetc(logfile)) != EOF) {
		waddch(dbgwin, c);
		usleep(100);
		wrefresh(dbgwin);
	}

	fseek(logfile, 0L, SEEK_END);
	dbgpos = ftell(logfile);
}

static void update_stack(WINDOW *win)
{
	int y, x;
	getyx(win, y, x);
	wmove(win, 7, 0);
	wprint_sc_stack(win, g_stack, sc_wprint_token);
	wmove(win, y, x);
	wrefresh(win);
}

static void print_file(WINDOW *win, const char *path)
{
	int c;
	FILE *f = fopen(path, "r");
	if (!f)
		cwprintw(win, "Could not open file '%s'!", path);

	while ((c = fgetc(f)) != EOF) {
		waddch(win, c);
		usleep(100);
		wrefresh(win);
	}
	fclose(f);
}

static void splash_banner(void)
{
	/* Banner in stdscr. */
	move((LINES - 8) / 2, 0);
	color_set(4, NULL);
	cwprintw(stdscr, "              ,,                                                             ");
	cwprintw(stdscr, "  .g8\"\"\"bgd `7MM                                   MMP\"\"MM\"\"YMM `7MMF' `YMM' ");
	cwprintw(stdscr, ".dP'     `M   MM                                   P'   MM   `7   MM   .M'   ");
	cwprintw(stdscr, "dM'       `   MMpMMMb.   .gP\"Ya  `7MMpMMMb.pMMMb.       MM        MM .d\"     ");
	cwprintw(stdscr, "MM            MM    MM  ,M'   Yb   MM    MM    MM       MM        MMMMM.     ");
	cwprintw(stdscr, "MM.           MM    MM  8M\"\"\"\"\"\"   MM    MM    MM       MM        MM  VMA    ");
	cwprintw(stdscr, "`Mb.     ,'   MM    MM  YM.    ,   MM    MM    MM       MM        MM   `MM.  ");
	cwprintw(stdscr, "  `\"bmmmd'  .JMML  JMML. `Mbmmd' .JMML  JMML  JMML.   .JMML.    .JMML.   MMb.");
	refresh();

	int y = getcury(stdscr) + 1;
	for (int x = 1; x <= COLS; x++) {
		mvaddstr(y,     x, "\u2593");
		mvaddstr(y + 1, x, "\u2593");
		mvaddstr(y + 2, x, "\u2593");
		usleep(1337 * 3);
		refresh();
	}
}

static void fill_win(WINDOW *win)
{
	int y, x;
	/* wchar_t f[] = { */
	/* 	/1* { <█>, <▓>, <▒>, <░>, */
	/* 	 * <$>, <#>, <%>, <&>, <*>, <+>, <:>, <;>, <,>, <.>, < > }. *1/ */
	/* 	9608, 9619, 9618, 9617, */
	/* 	36,   35,   37,   38,   42,   43,   58,   59,   44,   46,   32, */
	/* }; */
	wchar_t f[] = {
		9608, 9619, 9618, 9617,   32,
	}; /* { <█>, <▓>, <▒>, <░>, < > */
	size_t fills = sizeof(f) / sizeof(f[0]);

	getyx(win, y, x);
	wattron(win, A_DIM);
	for (size_t i = 0; i < fills; i++) {
		for (int j = 0; j < getmaxy(win); j++) {
			for (int k = 0; k < getmaxx(win); k++) {
				mvwprintw(win, j, k, "%lc", f[i]);
			}
			usleep(1337);
			wrefresh(win);
		}
	}
	wattroff(win, A_DIM);
	wmove(win, y, x);
}

static void shuffle(int x[], size_t n)
{
	srand(time(NULL));
	for (size_t i = 0; i < n - 1; i++) {
		size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
		int t = x[j];
		x[j] = x[i];
		x[i] = t;
	}
}

static void fill_win_random(WINDOW *win)
{
	int y, x;
	/* wchar_t f[] = { */
	/* 	/1* { <█>, <▓>, <▒>, <░>, */
	/* 	 * <$>, <#>, <%>, <&>, <*>, <+>, <:>, <;>, <,>, <.>, < > }. *1/ */
	/* 	9608, 9619, 9618, 9617, */
	/* 	36,   35,   37,   38,   42,   43,   58,   59,   44,   46,   32, */
	/* }; */
	/* wchar_t f[] = { */
	/* 	9608, 9619, 9618, 9617,   32, */
	/* }; /1* { <█>, <▓>, <▒>, <░>, < > *1/ */
	wchar_t f[] = {
		9619, 9618, 9617,   32,
	}; /* { <▓>, <▒>, <░>, < > */
	size_t fills = sizeof(f) / sizeof(f[0]);

	int *ry, *rx, ym, xm;
	ym = getmaxy(win);
	xm = getmaxx(win);
	ry = malloc(sizeof(int) * ym);
	rx = malloc(sizeof(int) * xm);
	if (!ry || !rx)
		return;

	for (int i = 0; i < ym; i++)
		ry[i] = i;
	for (int i = 0; i < xm; i++)
		rx[i] = i;

	getyx(win, y, x);
	wattron(win, A_DIM);

	for (int i = 0; i < ym; i++)
		for (int j = 0; j < xm; j++)
			mvwaddstr(win, i, j, "\u2588");
	wrefresh(win);

	for (size_t i = 0; i < fills; i++) {
		int j, k;
		j = k = 0;
		shuffle(ry, ym);
		shuffle(rx, xm);
		for (int l = 0; l < ym * xm; l++) {
			if (j == ym) {
				shuffle(ry, ym);
				j = 0;
			}
			if (k == xm) {
				shuffle(rx, xm);
				k = 0;
			}
			mvwprintw(win, ry[j++], rx[k++], "%lc", f[i]);
			if (l % 8 == 0)
				wrefresh(win);
		}
	}
	wattroff(win, A_DIM);
	wmove(win, y, x);

	free(ry);
	free(rx);
}

static void fill_win_rainbow(WINDOW *win, int colors)
{
	int y, x;
	wchar_t *f[] = {
		/* { <█> }. */
		(wchar_t []) { 9608, 0 },
		/* { <░>, <▒>, <▓>, <█> }. */
		(wchar_t []) { 9617, 9618, 9619, 9608, 0 },
		/* { <░>, <▒>, <▓> }. */
		(wchar_t []) { 9617, 9618, 9619, 0 },
		/* { <░>, <▒> }. */
		(wchar_t []) { 9617, 9618, 0 },
		/* { <░> }. */
		(wchar_t []) { 9617, 0 },
		/* { <░>, < > }. */
		(wchar_t []) { 9617, 32,   0 },
		/* { <:>, <.>, <,>, <;>, <%>, <+>, <#>, <$> }. */
		(wchar_t []) { 58,   46,   44,   59,   37,   43,   35,    36,   0 },
		/* { <:>, <.>, <,>, <;> }. */
		(wchar_t []) { 58,   46,   44,   0 },
		/* { < > }. */
		(wchar_t []) { 32,   0 },
	};
	size_t fills = sizeof(f) / sizeof(f[0]);

	getyx(win, y, x);
	wattron(win, A_DIM);
	for (size_t i = 0; i < fills; i++) {
		for (int j = 0; j < getmaxy(win); j++) {
			for (int k = 0; k < getmaxx(win); k++) {
				size_t len = 0;
				while (f[i][len++])
					;
				len--;
				wcolor_set(win, rand() % colors, NULL);
				mvwprintw(win, j, k, "%lc",
						f[i][rand() % len]);
			}
			usleep(420);
			wrefresh(win);
		}
	}
	wattroff(win, A_DIM);
	wmove(win, y, x);
}

static int init_ncurses(void)
{
	setlocale(LC_ALL, "");
	FILE *tty = fopen("/dev/tty", "r+");
	scr = newterm(NULL, tty, tty);
	start_color();
	raw();
	noecho();

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);

	if (LINES >= DBGHEIGHT) {
		if (COLS >= (2 * DBGWIDTH) + (DBGWIDTH / 2)) {
			dbgwin  = newwin(LINES, DBGWIDTH, 0, 0);
			infowin = newwin(LINES, DBGWIDTH, 0, COLS - DBGWIDTH);
			calcwin = newwin(LINES, COLS - (2 * DBGWIDTH), 0, DBGWIDTH);
		} else if (COLS >= DBGWIDTH + (DBGWIDTH / 2)) {
			dbgwin  = newwin(DBGHEIGHT, DBGWIDTH, LINES - DBGHEIGHT, COLS - DBGWIDTH);
			infowin = newwin(LINES - DBGHEIGHT, DBGWIDTH, 0, COLS - DBGWIDTH);
			calcwin = newwin(LINES, COLS - DBGWIDTH, 0, 0);
		} else if (COLS >= DBGWIDTH) {
			dbgwin  = newwin(DBGHEIGHT, COLS, LINES - DBGHEIGHT, 0);
			infowin = newwin(DBGHEIGHT, COLS, 0, 0);
			calcwin = newwin(LINES - (2 * DBGHEIGHT), COLS, DBGHEIGHT, 0);
		}
	} else if (LINES >= DBGHEIGHT / 2 && COLS >= DBGWIDTH) {
		dbgwin  = newwin(3, COLS, LINES - 3, 0);
		infowin = newwin(3, COLS, 0, 0);
		calcwin = newwin(LINES - (2 * 3), COLS, 3, 0);
		fputs("TOO FREAKING SMALL!\n", stderr);
		/* Too freaking small! Just make it all the calc window. */
		/* dbgwin  = newwin(1, 1, LINES - 1, COLS - 1); */
		/* infowin = newwin(1, 1, LINES, COLS); */
		/* calcwin = newwin(LINES, COLS, 0, 0); */
	} else {
		return 0;
	}

	keypad(calcwin, TRUE);

	scrollok(dbgwin,  true);
	scrollok(infowin, true);
	scrollok(calcwin, false);

	wcolor_set(dbgwin,  1, NULL);
	wcolor_set(infowin, 6, NULL);
	wcolor_set(calcwin, 5, NULL);

	/* box(dbgwin,  0, 0); */
	/* box(infowin, 0, 0); */
	/* box(calcwin, 0, 0); */
	/* fill_win(dbgwin); */
	/* fill_win(infowin); */
	/* fill_win(calcwin); */
	color_set(4, NULL);
	/* fill_win(stdscr); */
	/* fill_win_rainbow(stdscr, 7); */
	fill_win_random(stdscr);
	splash_banner();

	fill_win_random(dbgwin);
	fill_win_random(infowin);
	fill_win_random(calcwin);

	wrefresh(dbgwin);
	wrefresh(infowin);
	wrefresh(calcwin);

	dbgpos = 0L;

	return 1;
}

static void end_ncurses(void)
{
	delwin(dbgwin);
	delwin(infowin);
	delwin(calcwin);
	endwin();
	delscreen(scr);
}

cli_code run_cli(FILE *logfile)
{
	/* Parse command file. */
	if (parse_command_file(COMMAND_FILE, stderr) != 0) { /* Init. commands. */
		return CLI_FAIL_PARSE_CMD_FILE;
	} else if (!(g_stack = new_sc_stack())) { /* Init. stack. */
		return CLI_FAIL_INIT_SC_STACK;
	}
	print_sc_commands(stderr);
	if (!init_ncurses())
		return CLI_FAIL_INIT_CURSES;

	/* Banner in calcwin. */
	wmove(calcwin, 1, 0);
	cwprintw(calcwin, "  ____ _                  _____ _  __");
	cwprintw(calcwin, " / ___| |__   ___ _ __ __|_   _| |/ /");
	cwprintw(calcwin, "| |   | '_ \\ / _ \\ '_ ` _ \\| | | ' / ");
	cwprintw(calcwin, "| |___| | | |  __/ | | | | | | | . \\ ");
	cwprintw(calcwin, " \\____|_| |_|\\___|_| |_| |_|_| |_|\\_\\");
	wrefresh(calcwin);

	/* @todo: Start calculating (i.e. "scicalc"). */
	push_sc_stack(&g_stack, sc_tokenize("T"));
	push_sc_stack(&g_stack, sc_tokenize("ei n"));
	update_stack(calcwin);

	/* Init. and Help file. */
	print_file(infowin, SCICALC_DOCFILE);

	/* Print init. of commands. */
	cwprintw(dbgwin, "ChemTK SciCalc Version " VERSION ".\n\n");
	wprintw(dbgwin, "DATA_DIR: '" DATA_DIR "'.\n");
	wprintw(dbgwin, "COMMAND_LIB: '" COMMAND_LIB "'.\n\n");
	wprintw(dbgwin, "[ Parsing Command File... ]\n");
	wprintw(dbgwin, "[ Loaded commands! ]\n");
	update_bg(logfile);

	/* Wait for keypress before exit. */
	wgetch(calcwin);

	end_ncurses();
	return CLI_OK;
}

