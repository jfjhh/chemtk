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

#define SCICALC_NUMLEN NUMSTR_BUFSIZE
#define SCICALC_NUM_REGEXP "^ *[-+]?[0-9]+\\.?[0-9]*([eE][-+]?[0-9]+)? *$"

#define SCIWIN_COLOR_NORMAL 1
#define SCIWIN_COLOR_PROMPT 4

extern const char *title;

enum out_format { SCIENTIFIC, EXPANDED };

void scicalc(WINDOW *outwin, WINDOW *infowin);
int test_scicalc(WINDOW *outwin);

#endif /* SCICALC_H */

