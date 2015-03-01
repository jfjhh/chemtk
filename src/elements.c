#include "elements.h"

struct element *ptable[NUM_ELEMENTS];

static struct element *get_element(const char *element)
{
	FILE *file;
	struct element *read_element;
	int total;
	char *epath;
	size_t i;
	total = 0;

	if ((epath = (char *) malloc(sizeof(char) * ELEMENT_PATH_LEN)) == NULL) {
		free(epath);
		return NULL;
	}

	strncpy(epath, ELEMENT_PTABLE_DIR "/", strlen(ELEMENT_PTABLE_DIR "/") + 1);
	strncat(epath, element, ELEMENT_PATH_LEN);

	/* Convert to lowercase, element may be something like "Be". */
	for (i = 0; i < strlen(epath); i++)
		epath[i] = tolower(epath[i]);

	if ((file = fopen(epath, "r")) == NULL) {
		free(epath);
		return NULL;
	}

	if ((read_element =
				(struct element *) malloc(sizeof(struct element))) != NULL) {

		total += fscanf(file, "%s\n", read_element->name);
		total += fscanf(file, "%s\n", read_element->symbol);
		total += fscanf(file, "%f\n", &(read_element->molar_mass));
		total += fscanf(file, "%d\n", &(read_element->atomic_number));

		if (total != 4) {
			free(read_element);
			read_element = NULL;
		}
	}

	free(epath);
	return read_element;
}

static int valid_element(struct element *e)
{
	int status;

	if (e && e->name && e->symbol)
		status = 1;
	else
		status = 0;

	return status;
}

int print_element(struct element *e, WINDOW *outwin)
{
	int status;

	if ((status = valid_element(e)))
		wprintw(outwin,
				"%s (%s)\n"
				"Mass: %fg.\n"
				"Atomic Number: %d.\n\n",
				e->name, e->symbol,
				e->molar_mass,
				e->atomic_number);
	else
		wprintw(outwin,
				"Did not receive a valid element (print_element()).\n");

	return status;
}

int print_element_info(struct element *e, WINDOW *outwin)
{
	FILE *infofile;
	char *path, *symbol;
	int status;

	if (!(status = valid_element(e))) {
		wprintw(outwin,
				"Did not receive a valid element (print_element_info()).\n");
		return status;
	}

	path = (char *) malloc(sizeof(char) *
			(ELEMENT_INFO_DIR_LEN + ELEMENT_SYM_LEN) + 1);

	if (!path) {
		wprintw(outwin,
				"Not enough memory for path string in print_element_info()!\n");
		status = 0;
	}

	if (!(symbol = strndup(e->symbol, ELEMENT_SYM_LEN))) {
		wprintw(outwin,
				"Not enough memory for symbol string in print_element_info()!\n");
		status = 0;
	}

	if (path && symbol) {
		symbol[0] = (char) tolower(symbol[0]);

		strncpy(path, ELEMENT_INFO_DIR, ELEMENT_INFO_DIR_LEN);
		strncat(path, "/", 1);
		strncat(path, symbol, ELEMENT_SYM_LEN - 1);

		wprintw(outwin, "Element info path is '%s'.\n", path);

		if (!(infofile = fopen(path, "r"))) {
			wprintw(outwin, "Could not open element infofile at '%s'!\n", path);
			status = 0;
		} else {
			page_file(outwin, infofile);
			fclose(infofile);
		}
	}

	free(symbol);
	free(path);

	return status;
}

int print_ptable(WINDOW *outwin, int y, int x)
{
	FILE *file = fopen(PTABLE_FILE, "r");
	int status;

	if (file) {
		page_file_coords(outwin, file, y, x);
		status = 1;
	} else {
		mvwprintw(outwin, y, x, "[Unable to open ptable file!]\n");
		status = 0;
	}

	fclose(file);
	return status;
}

int init_elements(void)
{
	int element, ok;
	char **elements;

	ok = 1;
	if ((elements = get_files(ELEMENT_INFO_DIR)) != NULL) {
		for (element = 0; (elements[element] != NULL) && ok; element++) {
			if ((ptable[element] = get_element(elements[element])) == NULL) {
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
		if (strncmp(ptable[i]->symbol, symbol, ELEMENT_SYM_LEN) == 0) {
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

	/* Print all the elements and their info files. */
	if (!init_elements()) {
		wprintw(outwin, "Error with init_elements!\n");
		wgetch(outwin);
		status = 0;
	} else {
		status = 1;
		for (element = 0; element < NUM_ELEMENTS && status == 1; element++) {
			status = 0;
			status += valid_element(ptable[element]);

			/* Print ALL the info to the window */
#if (TEST_ELEMENTS_VERBOSE != 0)
			status += print_element(ptable[element], outwin);
			page_bottom(outwin);
			wrefresh(outwin);
			usleep(ELEMENT_AUTO_USEC);

			status += print_element_info(ptable[element], outwin);
			page_bottom(outwin);
			wrefresh(outwin);
			usleep(ELEMENT_AUTO_USEC);
#endif /* TEST_ELEMENTS_VERBOSE */
		}
	}
	end_elements();
	werase(outwin);

	if (!status) {
		wprintw(outwin, "Did not read all the elements properly!\n");
		wgetch(outwin);
		status = 0;
	} else {
		/* Print a periodic table. */
		werase(outwin);
		status = print_ptable_centered(outwin, 3);
		wrefresh(outwin);
#if (TEST_ELEMENTS_VERBOSE != 0)
		usleep(ELEMENT_PAUSE_USEC);
#endif /* TEST_ELEMENTS_VERBOSE */
	}

	return status;
}

