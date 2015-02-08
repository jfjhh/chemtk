#ifndef PAGER_H
#define PAGER_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define PAGER_COLOR_NORMAL 3

int page_line(WINDOW *outwin, FILE *file);

#endif

