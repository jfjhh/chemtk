/**
 * @file operator.h
 * @author Alex Striff
 *
 * @brief Checks to see if characters are supported mathematical operators.
 */

#ifndef OPERATOR_H
#define OPERATOR_H

#include <stdio.h>

/**
 * The number of operators that are supported.
 */
#define NUM_OPERATORS	16

/**
 * The number of operators tests that are ran.
 */
#define NUM_OPERATOR_TESTS	5

/**
 * The array of valid operators.
 */
extern const char operators[NUM_OPERATORS];

/**
 * Checks if @p op is a supported operator.
 *
 * @param op The @c char to check.
 *
 * @retval 0 @p op is not supported.
 * @retval 1 @p op is a supported operator.
 */
int is_operator(char op);

/**
 * Tests operator routines, logging results to @p logfile.
 *
 * @param logfile The file to log notes and the ongoing status of the test to.
 *
 * @retval 1 if test succeeded.
 * @retval 0 if test failed.
 */
int test_operator(FILE *logfile);

#endif /* OPERATOR_H */

