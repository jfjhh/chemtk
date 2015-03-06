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
#include "elements.h"
#include "operator.h"

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
#define SCICALC_NUM_REGEXP "^ *[-+]?[0-9]+\\.?[0-9]*([eE][-+]?[0-9]+)? *$"

#define SCIWIN_COLOR_NORMAL 1
#define SCIWIN_COLOR_PROMPT 4

/* Truncate a long double */
#define LDTOI(A) ((int)(A >= 0.0L ? (A + 0.5L) : (A - 0.5L)))

extern const char *title;

enum out_format { SCIENTIFIC, EXPANDED };

void scicalc(WINDOW *outwin, WINDOW *infowin);
int operate(WINDOW *outwin, struct stack *stack);
int test_scicalc(WINDOW *outwin);

#endif /* SCICALC_H */

