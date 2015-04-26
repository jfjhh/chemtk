#ifndef LD_MATH
#define LD_MATH

#include "constant.h"
#include "token.h"

/* Strange floating point values.
 * INFINITY and NINFINITY are defined in C99, and
 * NAN in IEEE Floating Point standard.
 */
#define _GNU_SOURCE

#ifndef INFINITY
#define INFINITY  (1.0 / 0.0)
#endif

#ifndef NINFINITY
#define NINFINITY (0 - INFINITY)
#endif

#ifndef NAN
#define NAN       -INFINITY
#endif

/* Truncate a long double. */
#define LDTOI(A)	((int)(A >= 0.0L ? (A + 0.5L) : (A - 0.5L)))

/* Find the smallest of the two values. */
#define least(A, B)	((A < B) ? A : B)

/* (S)ignificant (F)igure (C)ode. */
#define SFC_FIG_TESTS   4

/* Returns an array 3 ints long, with each index containing the significant
 * figures found in line in total, before the decimal, and after the decimal,
 * respectively (They can be accessed with foo[SFC_bar] as above).*/
int sig_figs(const char *line);

int test_ldmath(FILE *logfile);

#endif /* LD_MATH */

