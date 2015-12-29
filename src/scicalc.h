/**
 * @file scicalc.h
 * @author Alex Striff
 *
 * @brief Allows RPN style operation on a stack.
 */

#ifndef SCICALC_H
#define SCICALC_H

/* Feature test macro for fabsl(3). */
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif /* _BSD_SOURCE */

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
#include "stack.h"

/**
 * Performs an operation on @p stack (RPN calculation).
 *
 * @param stack The stack to operate on.
 *
 * Assumes a sc_stack with sc_tokens, and the tokens to be ordered:
 * @code
 * OPERATOR  <= top operator (like RPN).
 *  NUMBER   <= first operand.
 *  NUMBER   <= optional second operand.
 *   [...]   <= rest of stack.
 * @endcode
 *
 * @retval 0 if operation failed. @p stack may be unusable on error.
 * @retval 1 if operation succeeded.
 */
int operate(sc_stack **stack);

/**
 * Tests scicalc @c operate() routine, logging results to @p logfile.
 *
 * @param logfile The file to log notes and the ongoing status of the test to.
 *
 * @retval 1 if test succeeded.
 * @retval 0 if test failed.
 */
int test_scicalc(FILE *logfile);

#endif /* SCICALC_H */

