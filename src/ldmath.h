#ifndef LD_MATH
#define LD_MATH

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

#endif /* LD_MATH */

