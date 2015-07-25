/**
 * @file ldmath.h
 * @author Alex Striff
 *
 * @brief Provides routines for simple number comparisons and significant figure
 * calculations, as well as strange floating point values.
 *
 * Defines weird floating point numbers, if not present.
 * - INFINITY and NINFINITY are defined in C99.
 * - The **value** @c NaN is defined in the IEEE 754 Floating Point standard.
 * - The **macro** @c NAN is a GNU extension that works only on machines with
 * IEEE 754 floating point.
 *
 * @see https://www.gnu.org/software/libc/manual/html_node/Infinity-and-NaN.html
 */

#ifndef LDMATH_H
#define LDMATH_H

#ifndef _GNU_SOURCE // For floating point stuff.
#define _GNU_SOURCE
#endif /* _GNU_SOURCE */

#include <math.h>

#ifndef INFINITY
/**
 * Definition of @c INFINITY, if not present.
 */
#define INFINITY  (1.0 / 0.0)

/**
 * Synonym for definition of @c -INFINITY.
 */
#define NINFINITY -INFINITY
#endif /* INFINITY */

#ifndef NAN
/**
 * Replacement for standard definition of @c NAN, if not present.
 */
#define NAN       -INFINITY

#endif /* NAN */


/**
 * @def DTOI(A)
 * @brief Get the @c int representation of @p A.
 *
 * @param A The number to convert to an @c int.
 *
 * @retval int The number as an @c int.
 */
#define DTOI(A)        ((int)(A >= 0.0 ? (A + 0.5) : (A - 0.5)))

/**
 * @def least(A, B)
 * @brief Find the least of two numbers.
 *
 * @param A The first number to compare.
 * @param B The second number to compare.
 *
 * @returns
 * The least of the two numbers.
 */
#define least(A, B)    ((A < B) ? A : B)

/**
 * The number of significant figure tests.
 */
#define SFC_FIG_TESTS  4

/**
 * The number of members in a @link sig_fig_array <tt>significant figure
 * array</tt>@endlink.
 */
#define	SIG_FIG_LEN 3

/**
 * @def SIG_FIG_SET(A, B)
 * @brief Sets all fields of a @link sig_fig_array <tt>significant figure
 * array</tt>@endlink.
 *
 * @param A The @link sig_fig_array <tt>significant figure array</tt>@endlink to
 * operate on.
 * @param B The value to set all fields to.
 */
#define SIG_FIG_SET(A, B)	A[ALL] = A[BEFORE] = A[AFTER] = B;

/**
 * Used to index the @link sig_fig_array <tt>significant figure array</tt>@endlink
 * returned by @c sig_figs().
 */
enum sig_fig_field {
	ALL = 0,    /**< All significant figures of the number, suitable for use in
				  calculating multiplication or division significant figures. */

	BEFORE = 1, /**< All significant figures before the decimal place. */

	AFTER = 2,  /**< All significant figures after the decimal place, suitable for
				  calculating addition or subtraction significant figures. */
};

#include <stdlib.h>

#include "constant.h"
#include "token.h"

/**
 * Check that @p line is a valid representation of a number.
 *
 * @param line The string to check.
 *
 * @retval 1 The @p line is a valid representation of a number.
 * @retval 0 The @p line is an invalid representation of a number.
 */
int valid_num(const char *line);

/**
 * Gets significant figure information from @p line, accounting for constants
 * via a terminating @c SC_CONST_CHAR, and storing the results in an array.
 *
 * @param line The line to get the significant figures from. A string
 * representation of a number.
 *
 * @param figs The array to store significant figure information in. It must be
 * @c SIG_FIG_LEN long.
 *
 * @retval -1
 * If the number is a constant with unlimited significant figures.
 * All fields of @p figs is set to @c -1.
 *
 * @retval figs[ALL]
 * The number of significant figures found in total.
 *
 * @retval 0
 * The number string given is invalid.
 * All fields of @p figs is set to @c 0.
 *
 * @code{.c}
 * int figs[SIG_FIG_LEN];
 * sig_figs("013.230", figs);
 *
 * figs[ALL] => 5
 * figs[BEFORE] => 2
 * figs[AFTER] => 3
 * @endcode
 *
 * A
 * @anchor sig_fig_array <tt>significant figure array</tt> is an array of
 * @c ints that is @c SIG_FIG_LEN long, that has the purpose of recording
 * significant figure data on a number.
 * @see sig_fig_field for what each index of the array will contain.
 */
int sig_figs(const char *line, int *figs);

/**
 * Tests math routines, logging results to @p logfile.
 *
 * @param logfile The file to log notes and the ongoing status of the test to.
 * @retval 1 if test succeeded.
 * @retval 0 if test failed.
 */
int test_ldmath(FILE *logfile);

#endif /* LDMATH_H */

