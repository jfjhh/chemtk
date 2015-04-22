#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE	200809L
#endif

#include <ncurses.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dirs.h"

#define NUM_CONSTANTS	5
extern const char *constants[NUM_CONSTANTS][2];

long double const_search(const char *line);
int test_constants(WINDOW *outwin);

#endif /* CONSTANTS_H */

