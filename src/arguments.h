/**
 * @file arguments.h
 * @author Alex Striff
 *
 * @brief Handles arguments to main.
 */

#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#if ! (_POSIX_C_SOURCE >= 2) /* For getopt(3). */
#define _POSIX_C_SOURCE	2
#endif /* _POSIX_C_SOURCE */

#ifndef _GNU_SOURCE /* For getopt_long(3). */
#define _GNU_SOURCE
#endif /* _GNU_SOURCE */

#include <unistd.h>
#include <getopt.h>

/**
 * Global short options.
 */
static const char *const g_short_opts = "l::";

/**
 * Global long options.
 */
static struct option g_long_opts[] = {
	/**
	 * Logfile option.
	 */
	{
		.name    = "logfile",
		.has_arg = required_argument,
		.flag    = NULL, /**< @c val is unchanged. */
		.val     = 'l',
	},
	{ 0, 0, 0, 0, }, /**< Required zeroed last element. */
};

#endif /* ARGUMENTS_H */

