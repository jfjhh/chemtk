#ifndef PAGER_H
#define PAGER_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dirs.h"
#include "directory.h"

#define PAGER_COLOR_NORMAL 3
#define PAGER_STR "see next page"
/* #define PAGER_AUTO_USEC 5000 */
#define PAGER_AUTO_USEC 1000

void page_bottom(WINDOW *outwin);

void page_prompt(WINDOW *outwin, const char *prompt);
void page_prompt_bottom(WINDOW *outwin, const char *prompt);
#define page_prompt_file(A) page_prompt(A, PAGER_STR)

void page_file(WINDOW *outwin, FILE *file);
void page_file_coords(WINDOW *outwin, FILE *file, int y, int x);

#define page_file_auto(A, B) page_file_delay(A, B, PAGER_AUTO_USEC)
void page_file_delay(WINDOW *outwin, FILE *file, useconds_t time);

int test_pager(WINDOW *outwin);

#endif

