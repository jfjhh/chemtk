#ifndef PAGER_H
#define PAGER_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE	200809L
#endif

#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif

#define PAGER	"less"

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

/* Returns 0 on error and 1 on success. */
int page_file_stream(FILE *path);  /* More overhead. */
int page_file(const char *path);   /* Preferred. */

/* Returns 0 on error and 1 on success. */
int test_pager(FILE *logfile);

#endif

