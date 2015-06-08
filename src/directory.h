/**
 * @file directory.h
 * @author Alex Striff
 *
 * @brief Provides a basic @c ls like utility.
 */

#ifndef DIRECTORY_H
#define DIRECTORY_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif /* _GNU_SOURCE */

#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dirs.h"

/**
 * Gets a list of files in a directory, similar to @c ls.
 *
 * @param dir
 * The directory to search.
 *
 * @retval NULL
 * On error.
 * @retval char **
 * An array of pointers to string file names found in @p dir.
 *
 * @note The array of filenames returned to the caller must be freed with
 * free_filelist().
 */
char **get_files(const char *dir);

/**
 * Gets a list of files in a directory, and strips the file extensions at the
 * same time.
 *
 * @param dir
 * The directory to search.
 *
 * @retval NULL
 * On error.
 * @retval char **
 * An array of pointers to string file names found in @p dir.
 *
 * @note The array of filenames returned to the caller must be freed with
 * free_filelist().
 */
char **get_files_strip(const char *dir);

/**
 * Free all the memory of the file list.
 *
 * @param files
 * The list of files to free from memory.
 */
void free_filelist(char **files);

/**
 * Tests directory routines, logging results to @p logfile.
 *
 * @param logfile The file to log notes and the ongoing status of the test to.
 *
 * @retval 1 if test succeeded.
 * @retval 0 if test failed.
 */
int test_directory(FILE *logfile);

#endif /* DIRECTORY_H */

