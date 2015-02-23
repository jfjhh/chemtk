#ifndef DIRECTORY_H
#define DIRECTORY_H

#define _GNU_SOURCE

#include <ncurses.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
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

int test_directory(WINDOW *outwin);

#endif /* DIRECTORY_H */

