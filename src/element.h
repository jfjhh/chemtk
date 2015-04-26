#ifndef ELEMENTS_H
#define ELEMENTS_H

#if ! defined(_XOPEN_SOURCE) || _XOPEN_SOURCE < 700
#define _XOPEN_SOURCE	700
#endif

#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif

#if (TEST_ELEMENTS_VERBOSE != 0)
#	include <unistd.h>
#endif /* TEST_ELEMENTS_VERBOSE */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dirs.h"
#include "directory.h"
#include "pager.h"

#define NUM_ELEMENTS        118
#define ELEMENT_NAME_LEN    32
#define ELEMENT_SYM_LEN     4

#define ELEMENT_AUTO_USEC   1000
#define ELEMENT_PAUSE_USEC  500000

struct element {
	char  name[ELEMENT_NAME_LEN];
	char  symbol[ELEMENT_SYM_LEN];
	float molar_mass;
	int   atomic_number;
};

extern struct element *ptable[NUM_ELEMENTS];

/* Prints the data about an element to file. */
int print_element(struct element *e, FILE *file);

/* Prints a longer description of the element to file. */
int print_element_info(struct element *e, FILE *file);

/* Prints an ASCII Periodic table with the top-left corner at (x,y). The
 * centered version only needs a y-coordinate. */
int print_ptable(FILE *file);

/* Read the entire periodic table into memory. */
int init_elements(void);

/* Free the entire periodic table from memory. */
void end_elements(void);

/* Returns a pointer to the found element in ptable, or NULL if not found.
 * Search using the element's symbol, or name function, respectively. */
struct element *find_element_sym(const char *symbol);
struct element *find_element_name(const char *name);

int test_element(FILE *logfile);

#endif /* ELEMENTS_H */

