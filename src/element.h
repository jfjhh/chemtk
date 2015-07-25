/**
 * @file element.h
 * @author Alex Striff
 *
 * @brief Routines for getting information about chemical elements.
 *
 * Information that can be gained ranges from specific data, such as atomic
 * mass, to detailed paragraphs on an element, to printing out an ASCII
 * periodic table.
 */

#ifndef ELEMENTS_H
#define ELEMENTS_H

#if ! defined(_XOPEN_SOURCE) || _XOPEN_SOURCE < 700 // For strndup()
#define _XOPEN_SOURCE	700
#endif

#ifndef _BSD_SOURCE // For snprintf()
#define _BSD_SOURCE
#endif

/**
 * The number of elements known about by the program.
 */
#define NUM_ELEMENTS        118

/**
 * The maximum buffer length for the name of an element, including the
 * terminating @c NULL byte.
 */
#define ELEMENT_NAME_LEN    32

/**
 * The maximum buffer length for the symbol of an element, including the
 * terminating @c NULL byte.
 */
#define ELEMENT_SYM_LEN     4

/**
 * The time that the program will sleep for when automatically displaying
 * element information, in microseconds.
 *
 * @note Will only be used if @c TEST_ELEMENTS_VERBOSE is nonzero.
 */
#define ELEMENT_AUTO_USEC   1000

/**
 * The time that the program will sleep for when waiting at the end of
 * automatically displaying element information, in microseconds.
 *
 * @note Will only be used if @c TEST_ELEMENTS_VERBOSE is nonzero.
 */
#define ELEMENT_PAUSE_USEC  500000

#if (TEST_ELEMENTS_VERBOSE != 0)
#	include <unistd.h>
#endif /* TEST_ELEMENTS_VERBOSE */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dirs.h"
#include "directory.h"
#include "pager.h"

/**
 * Structure that contains all relevant information about a chemical element.
 */
struct element {
	char  name[ELEMENT_NAME_LEN];  /**< The element's name. */
	char  symbol[ELEMENT_SYM_LEN]; /**< The element's chemical symbol. */
	float molar_mass;              /**< The element's molar mass. */
	int   atomic_number;           /**< The element's atomic number. */
};

/**
 * Universal index of all elements in the periodic table that is loaded into
 * memory on runtime and used as a reference to get all individual element
 * information.
 * @anchor ptable
 */
extern struct element *ptable[NUM_ELEMENTS];

/**
 * Prints the basic data about element @p e to @p file.
 *
 * @param e The element to print.
 * @param file The file to print to.
 *
 * @retval 1 The element printed was valid and printed correctly.
 * @retval 0 The element could not be printed, and an error was printed
 * instead.
 */
int print_element(struct element *e, FILE *file);

/**
 * Prints the long description of element @p e to @p file.
 *
 * @param e The element to print.
 * @param file The file to print to.
 *
 * @retval 1 The element printed was valid and printed correctly.
 * @retval 0 The element could not be printed, and an error was printed
 * instead.
 */
int print_element_info(struct element *e, FILE *file);

/**
 * Prints an ASCII periodic table to @p file.
 *
 * @param file The file to print to.
 *
 * @retval 1 The periodic table was printed successfully.
 * @retval 0 The file containing the periodic table could not be read, and an
 * error was printed.
 */
int print_ptable(FILE *file);

/**
 * Reads the entire periodic table into memory.
 *
 * @retval 1 Success.
 * @retval 0 An error occured.
 *
 * @note Operates on @link ptable @c ptable @endlink.
 */
int init_elements(void);

/**
 * Frees the periodic table.
 *
 * @note Operates on @link ptable @c ptable @endlink.
 */
void end_elements(void);

/**
 * Finds an element in @link ptable @c ptable by element symbol @endlink.
 *
 * @param symbol The symbol of the element to search for.
 *
 * @retval struct element *
 * The found element in ptable.
 * @retval NULL
 * If the element is not found.
 */
struct element *find_element_sym(const char *symbol);

/**
 * Finds an element in @link ptable @c ptable by element name @endlink.
 *
 * @param name The name of the element to search for.
 *
 * @retval struct element *
 * The found element in ptable.
 * @retval NULL
 * If the element is not found.
 */
struct element *find_element_name(const char *name);

/**
 * Tests element routines, logging results to @p logfile.
 *
 * @param logfile The file to log notes and the ongoing status of the test to.
 *
 * @retval 1 if test succeeded.
 * @retval 0 if test failed.
 */
int test_element(FILE *logfile);

#endif /* ELEMENTS_H */

