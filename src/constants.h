#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CONST_FILE "doc/constants.txt"

long double const_search(const char *line);
int test_constants(WINDOW *outwin);

#endif /* CONSTANTS_H */

