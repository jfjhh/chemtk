#ifndef DIRECTORY_H
#define DIRECTORY_H

#define _GNU_SOURCE

#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dirs.h"

/*
 * Returns an array of pointers to string file names.
 * The array of filenames returned to the caller must be freed with
 * free_filenames().
 * Returns NULL on error.
 */
char **get_files(const char *dir);

/*
 * Same as get_files(), but get rid of the file extension.
 * (e.g. "blah.txt" => "blah").
 */
char **get_files_strip(const char *dir);

/*
 * Free all the memory of the file list.
 */
void free_filelist(char **files);

/* Tests directory funtions. */
int test_directory(FILE *logfile);

#endif /* DIRECTORY_H */

