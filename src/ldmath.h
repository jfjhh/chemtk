#ifndef LD_MATH
#define LD_MATH

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

/* Truncate a long double. */
#define LDTOI(A) ((int)(A >= 0.0L ? (A + 0.5L) : (A - 0.5L)))

/* Find the smallest of the two values. */
#define least(A, B) ((A < B) ? A : B)

#endif /* LD_MATH */

