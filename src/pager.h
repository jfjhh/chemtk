#ifndef PAGER_H
#define PAGER_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define PAGER_COLOR_NORMAL 3
#define TEST_PAGER_FILE "data/pager.txt"
#define PAGER_STR "see next page"

void page_bottom(WINDOW *outwin);

void page_prompt(WINDOW *outwin, const char *prompt);
void page_prompt_bottom(WINDOW *outwin, const char *prompt);
#define page_prompt_file(A) page_prompt(A, PAGER_STR)

void page_file(WINDOW *outwin, FILE *file);
void page_file_coords(WINDOW *outwin, FILE *file, int y, int x);

int test_pager(WINDOW *outwin);

#endif

