#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <ncurses.h>

struct element {
	char *name;
	char *symbol;
	float molar_mass;
	int atomic_number;
};

int test_elements(WINDOW *outwin);

#endif /* ELEMENTS_H */
