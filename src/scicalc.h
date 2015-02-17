#ifndef SCICALC_H
#define SCICALC_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include <ncurses.h>

#include "stack.h"

/* Strange floating point values. */
#define _GNU_SOURCE
#ifndef INFINITY /* ISO C99 standard. */
#define INFINITY 1.0 / 0.0
#endif
#ifndef NINFINITY /* ISO C99 standard. */
#define NINFINITY 0 - INFINITY
#endif
#ifndef N0 /* IEEE 754 */
#define N0 1.0 / -INFINITY
#endif
#ifndef NAN /* IEEE floating point. */
#define NAN -INFINITY
#endif

#define SCICALC_DOCFILE DATA_DIR "/scicalc.txt"
#define SCIWIN_COLOR_NORMAL 1
#define LINE_LEN 32

extern const char *title;

#define NUM_CONSTANTS 5
extern const char *constants[NUM_CONSTANTS][2];

#define NUM_OPERATORS 16
extern const char operators[NUM_OPERATORS];

enum out_format { SCIENTIFIC, EXPANDED };
int get_sig(const char *line);
long double operate(long double a, long double b, char operation);
int test_scicalc(WINDOW *outwin);

#endif /* SCICALC_H */

