/**
 * @file pager.h
 * @author Alex Striff
 *
 * @brief Pages files.
 */

#ifndef PAGER_H
#define PAGER_H

#ifndef _POSIX_SOURCE // For fileno()
#define _POSIX_SOURCE
#endif /* _POSIX_SOURCE */

#ifndef _BSD_SOURCE // For snprintf()
#define _BSD_SOURCE
#endif

/**
 * Aliases for true or false automatic paging exit preferences.
 */
enum page_exit {
	MANUAL_PAGE  = 0, /**< Manually exit the pager. */
	AUTO_PAGE    = 1  /**< Auto-exit on EOF. */
};

/**
 * The pager program used, present in the user's @c PATH.
 */
#define SC_PAGER	"less"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "dirs.h"

/**
 * Pages the file at @p path via @c SC_PAGER, manually.
 *
 * Lazy way of doing the following:
 * @code
 * page_file_exit(file, MANUAL_PAGE);
 * @endcode
 */
#define page_file(A)	page_file_exit(A, MANUAL_PAGE)

/**
 * Pages the file at @p path via @c SC_PAGER, automatically.
 *
 * Lazy way of doing the following:
 * @code
 * page_file_exit(file, AUTO_PAGE);
 * @endcode
 */
#define page_auto(A)	page_file_exit(A, AUTO_PAGE)

/**
 * Pages the file at @p path via @c SC_PAGER.
 *
 * @param path The path to the file to page.
 *
 * @retval 1 If paged successfully.
 * @retval 0 An error occured. An error message was printed.
 *
 * @note This function is preferred over @c page_file_stream().
 */
int page_file_exit(const char *path, enum page_exit auto_exit);

/**
 * Pages the file stream @p path via @c SC_PAGER.
 *
 * @param path The file stream of the file to page.
 *
 * @retval 1 If paged successfully.
 * @retval 0 An error occured. An error message was printed.
 *
 * @note This function is depreciated if @c page_file() can be used instead, as
 * it jumps through some hoops to get a file path from a file stream, a process
 * that can sometimes be unreliable depending on limits of the system.
 */
int page_file_stream(FILE *path);

/**
 * Tests paging routines, logging results to @p logfile.
 *
 * @param logfile The file to log notes and the ongoing status of the test to.
 *
 * @retval 1 if test succeeded.
 * @retval 0 if test failed.
 */
int test_pager(FILE *logfile);

#endif

