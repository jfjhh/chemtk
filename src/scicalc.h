#ifndef SCICALC_H
#define SCICALC_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>

#include "dirs.h"
#include "stack.h"
#include "directory.h"
#include "token.h"
#include "element.h"
#include "operator.h"

/* void scicalc(FILE *logfile); */
int operate(sc_stack **stack);
int test_scicalc(FILE *logfile);

#endif /* SCICALC_H */

