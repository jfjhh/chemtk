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

/* Assumes a sc_stack with sc_tokens, and the tokens to be ordered:
 * OPERATOR  <= top operator (like RPN).
 * NUMBER    <= first operand.
 * (NUMBER)  <= optional second operand.
 *  [...]    <= rest of stack.
 */
int operate(sc_stack **stack);

int test_scicalc(FILE *logfile);

#endif /* SCICALC_H */

