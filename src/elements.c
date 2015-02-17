#include "elements.h"

struct element *ptable[NUM_ELEMENTS];

static struct element *get_element(const char *element)
{
	FILE *file;
	struct element *read_element;
	int total = 0;
	char *epath;

	if ((epath = (char *) malloc(sizeof(char) * ELEMENT_PATH_LEN)) == NULL) {
		/* fprintf(stderr, "Could not allocate memory for path string (epath).\n"); */
		free(epath);
		return NULL;
	}

	strncpy(epath, ELEMENT_PTABLE_DIR "/", strlen(ELEMENT_PTABLE_DIR "/") + 1);
	strncat(epath, element, ELEMENT_PATH_LEN);

	if ((file = fopen(epath, "r")) == NULL) {
		/* fprintf(stderr, "Could not open element file `%s' (epath: `%s').\n", */
		/* 		element, epath); */
		free(epath);
		return NULL;
	}

	if ((read_element =
				(struct element *) malloc(sizeof(struct element))) != NULL) {

		total += fscanf(file, "%s\n", read_element->name);
		/* fprintf(stderr, "Read name: %s\n", read_element->name); */

		total += fscanf(file, "%s\n", read_element->symbol);
		/* fprintf(stderr, "Read symbol: %s\n", read_element->symbol); */

		total += fscanf(file, "%f\n", &(read_element->molar_mass));
		/* fprintf(stderr, "Read m. mass: %f\n", read_element->molar_mass); */

		total += fscanf(file, "%d\n", &(read_element->atomic_number));
		/* fprintf(stderr, "Read a. number: %d\n", read_element->atomic_number); */

		/* fputc('\n', stderr); */

		if (total != 4) {
			/* fprintf(stderr, */
			/* 		"Did not read the correct number of items from element file"); */
			free(read_element);
			read_element = NULL;
		}
	}

	free(epath);
	return read_element;
}

void print_element(struct element *e, WINDOW *outwin)
{
	wprintw(outwin,
			"%s (%s)\n"
			"Mass: %fg.\n"
			"Atomic Number: %d.\n\n",
			e->name, e->symbol,
			e->molar_mass,
			e->atomic_number);
}

void print_ptable(WINDOW *outwin, int y, int x)
{
	FILE *file = fopen(PTABLE_FILE, "r");

	if (file)
		page_file_coords(outwin, file, y, x);
	else
		mvwprintw(outwin, y, x, "[Unable to open ptable file!]\n");

	fclose(file);
}

int init_elements(void)
{
	int element, ok;
	char **elements;

	ok = 1;
	if ((elements = get_files(ELEMENT_INFO_DIR)) != NULL) {
		for (element = 0; (elements[element] != NULL) && ok; element++) {
			if ((ptable[element] = get_element(elements[element])) == NULL) {
				/* fprintf(stderr, "Bad element in init_elements: %p.\n", */
				/* 		(void *) ptable[element]); */
				ok = 0;
			}
		}
	} else {
		ok = 0;
	}

	free_filelist(elements);
	return ok;
}

void end_elements(void)
{
	int i;

	for (i = 0; i < NUM_ELEMENTS; i++)
		free(ptable[i]);
}

struct element *find_element_sym(const char *symbol)
{
	struct element *e = NULL;
	int i, found;

	found = 0;
	for (i = 0; i < NUM_ELEMENTS && !found; i++) {
		if (strncmp(symbol, ptable[i]->symbol, ELEMENT_SYM_LEN) == 0) {
			e = ptable[i];
			found = 1;
		}
	}

	return e;
}

struct element *find_element_name(const char *name)
{
	struct element *e = NULL;
	int i, found;

	found = 0;
	for (i = 0; i < NUM_ELEMENTS && !found; i++) {
		if (strncmp(name, ptable[i]->name, ELEMENT_SYM_LEN) == 0) {
			e = ptable[i];
			found = 1;
		}
	}

	return e;
}

int test_elements(WINDOW *outwin)
{
	int status, element;
	status = 1;

	/* Print all the elements. */
	if (!init_elements()) {
		/* fprintf(stderr, "Error with init_elements!\n"); */
		status = 0;
	} else {
		for (element = 0; element < NUM_ELEMENTS; element++) {
			print_element(ptable[element], outwin);
			usleep(10000);
			wrefresh(outwin);
			page_bottom(outwin);
		}
	}

	/* End elements and print a periodic table. */
	end_elements();
	page_prompt(outwin, "see a periodic table");
	werase(outwin);

	print_ptable(outwin, 4, 4);
	page_prompt(outwin, "end this test");

	return status;
}

