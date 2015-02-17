#ifndef ELEMENTS_H
#define ELEMENTS_H

#if ! defined(_XOPEN_SOURCE) || _XOPEN_SOURCE < 500
#define _XOPEN_SOURCE 500
#endif

#include <ncurses.h>
#include <unistd.h>

#include "directory.h"
#include "pager.h"

#define ELEMENT_DIR "data/elements"

#define ELEMENT_INFO_DIR ELEMENT_DIR "/info"
#define ELEMENT_PTABLE_DIR ELEMENT_DIR "/ptable"
#define ELEMENT_PTABLE_DIR_LEN strlen(ELEMENT_PTABLE_DIR) + 1

#define PTABLE_FILE ELEMENT_DIR "/ptable.txt"

#define NUM_ELEMENTS 118
#define ELEMENT_NAME_LEN 32
#define ELEMENT_SYM_LEN 4

#define ELEMENT_PATH_LEN (ELEMENT_SYM_LEN + 1 + ELEMENT_PTABLE_DIR_LEN)

struct element {
	char name[ELEMENT_NAME_LEN];
	char symbol[ELEMENT_SYM_LEN];
	float molar_mass;
	int atomic_number;
};

extern struct element *ptable[NUM_ELEMENTS];

/* Prints the data about an element to outwin. */
void print_element(struct element *e, WINDOW *outwin);

/* Prints an ASCII Periodic table with the top-left corner at (x,y). */
void print_ptable(WINDOW *outwin, int y, int x);

/* Read the entire periodic table into memory. */
int init_elements(void);

/* Free the entire periodic table from memory. */
void end_elements(void);

/* Returns a pointer to the found element in ptable, or NULL if not found.
 * Search using the element's symbol, or name function, respectively. */
struct element *find_element_sym(const char *symbol);
struct element *find_element_name(const char *name);

int test_elements(WINDOW *outwin);

#endif /* ELEMENTS_H */
