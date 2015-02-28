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
#include <form.h>

#include "dirs.h"
#include "stack.h"
#include "directory.h"
#include "num_str.h"

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

#define SCICALC_NUMLEN NUMSTR_BUFSIZE
#define SCICALC_NUM_REGEXP "^ *[-+]?[0-9]*\\.?[0-9]*([eE][-+]?[0-9]+)? *$"

#define SCIWIN_COLOR_NORMAL 1
#define SCIWIN_COLOR_PROMPT 4

extern const char *title;

#define NUM_CONSTANTS 5
extern const char *constants[NUM_CONSTANTS][2];

#define NUM_OPERATORS 16
extern const char operators[NUM_OPERATORS];

enum out_format { SCIENTIFIC, EXPANDED };
void scicalc(WINDOW *outwin);
long double operate(long double a, long double b, char operation);
int test_scicalc(WINDOW *outwin);

#endif /* SCICALC_H */

