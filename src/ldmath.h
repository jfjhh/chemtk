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

#endif /* LD_MATH */

