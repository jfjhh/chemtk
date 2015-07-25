/**
 * @file constant.h
 * @author Alex Striff
 *
 * @brief Provides lookup of constant values by string name.
 */

#ifndef CONSTANT_H
#define CONSTANT_H

#ifndef _POSIX_C_SOURCE // For getline()
#define _POSIX_C_SOURCE	200809L
#endif

/**
 * The character that delimits a constant when appended to a string
 * representation of a number.
 *
 * @note So @c "22.4." would be interpreted as a constant, while @c "22.4" would
 * not be seen as a constant.
 */
#define SC_CONST_CHAR	'.'

/**
 * The number of constants that are known in @c CONST_FILE.
 */
#define NUM_CONSTANTS	5

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dirs.h"

/**
 * Searches through CONST_FILE for the name of the constant given in @p line,
 * and returns it as a number.
 *
 * @param line
 * The string to find the constant value for.
 *
 * @return
 * The constant as a double.
 *
 * E.g. const_search("mol"); => @c 6.022141e+23
 */
double const_search(const char *line);

/**
 * Tests constants, logging results to @p logfile.
 *
 * @param logfile The file to log notes and the ongoing status of the test to.
 *
 * @retval 1
 * If test succeeded.
 * @retval 0
 * If test failed.
 */
int test_constant(FILE *logfile);

#endif /* CONSTANT_H */

