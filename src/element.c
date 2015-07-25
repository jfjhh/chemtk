/**
 * @file element.c
 * @author Alex Striff
 *
 * @brief Routines for getting information about chemical elements.
 *
 * Information that can be gained ranges from specific data, such as atomic
 * mass, to detailed paragraphs on an element, to printing out an ASCII
 * periodic table.
 */

#include "element.h"

struct element *ptable[NUM_ELEMENTS];

static struct element *get_element(const char *element)
{
	FILE *file;
	struct element *read_element;
	int total;
	char *epath;
	size_t i;
	total = 0;

	if ((epath = malloc(sizeof(char) * ELEMENT_PATH_LEN)) == NULL) {
		free(epath);
		return NULL;
	}

	snprintf(epath, ELEMENT_PATH_LEN + 1 + ELEMENT_PTABLE_DIR_LEN, // +1 for '/'
			ELEMENT_PTABLE_DIR "/%s", element);

	/* Convert to lowercase, element may be something like "Be". */
	for (i = 0; i < strlen(epath); i++)
		epath[i] = tolower(epath[i]);

	if ((file = fopen(epath, "r")) == NULL) {
		free(epath);
		perror("get_element: could not open element file for reading");
		return NULL;
	}

	if ((read_element =
				malloc(sizeof(struct element))) != NULL) {

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
	fclose(file);
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

int print_element(struct element *e, FILE *file)
{
	int status;

	if ((status = valid_element(e)))
		fprintf(file,
				"%s (%s)\n"
				"Mass: %fg.\n"
				"Atomic Number: %d.\n\n",
				e->name, e->symbol,
				e->molar_mass,
				e->atomic_number);
	else
		fprintf(file,
				"Did not receive a valid element (print_element()).\n");

	return status;
}

int print_element_info(struct element *e, FILE *file)
{
	char *path, *symbol;
	int status;

	if (!(status = valid_element(e))) {
		fprintf(file,
				"Did not receive a valid element (print_element_info()).\n");
		return status;
	}

	path = malloc(sizeof(char) *
			(ELEMENT_INFO_DIR_LEN + ELEMENT_SYM_LEN) + 1);

	if (!path) {
		fprintf(file,
				"Not enough memory for path string in print_element_info()!\n");
		status = 0;
	}

	if (!(symbol = strndup(e->symbol, ELEMENT_SYM_LEN))) {
		fprintf(file,
				"Not enough memory for symbol string in print_element_info()!\n");
		status = 0;
	}

	if (path && symbol) {
		symbol[0] = (char) tolower(symbol[0]);

		snprintf(path, ELEMENT_INFO_DIR_LEN + 1 + ELEMENT_SYM_LEN, // +1 for '/'
				ELEMENT_INFO_DIR "/%s", symbol);

		fprintf(file, "Element info path is '%s'.\n", path);
		page_file(path);
	}

	free(symbol);
	free(path);

	return status;
}

int print_ptable(FILE *file)
{
	FILE *ptable_file = fopen(PTABLE_FILE, "r");
	int status, c;

	if (file) {
		while ((c = fgetc(ptable_file)) != EOF)
			fputc(c, file);
		status = 1;
	} else {
		fprintf(file, "Unable to print ptable file!\n");
		status = 0;
	}

	fclose(ptable_file);
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

int test_element(FILE *logfile)
{
	int status, element;

	/**
	 * @test
	 * Tests the ability to load all elements into memory.
	 */
	if (!init_elements()) {
		fprintf(logfile, "Error with init_elements!\n");
		status = 0;
	} else {
		status = 1;
		for (element = 0; element < NUM_ELEMENTS && status == 1; element++) {

			/**
			 * @test
			 * Validates all elements.
			 */
			status = 0;
			status += valid_element(ptable[element]);

			/**
			 * @test
			 * Tests if it is possible to print all the elements and their info
			 * files.
			 * @note
			 * Only when @c TEST_ELEMENTS_VERBOSE is nonzero.
			 */
#if TEST_ELEMENTS_VERBOSE
			status += print_element(ptable[element], file);
			usleep(ELEMENT_AUTO_USEC);

			status += print_element_info(ptable[element], file);
			page_bottom(file);
			usleep(ELEMENT_AUTO_USEC);
#endif /* TEST_ELEMENTS_VERBOSE */
		}
	}
	end_elements();

	if (!status) {
		fprintf(logfile, "Did not read all the elements properly!\n");
		status = 0;
	} else {
		/* Print a periodic table. */
		status = print_ptable(logfile);
#if TEST_ELEMENTS_VERBOSE
		usleep(ELEMENT_PAUSE_USEC);
#endif /* TEST_ELEMENTS_VERBOSE */
	}

	return status;
}

