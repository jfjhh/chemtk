#ifndef CONSTANT_H
#define CONSTANT_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE	200809L
#endif

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dirs.h"

#define SC_CONST_CHAR '.'

#define NUM_CONSTANTS	5
extern const char *consant[NUM_CONSTANTS][2];

double const_search(const char *line);
int test_consant(FILE *logfile);

#endif /* CONSTANT_H */

