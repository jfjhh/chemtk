/**
 * @file input.h
 * @author Alex Striff
 *
 * @brief Specifies the command *input* datastructures. By *input*, it is meant
 * the type of input that a command takes. This is specified by a name
 * ("symbol", "none", "number", etc.) and a maximum string length.
 */

#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <yaml.h>

#include "dirs.h"

/**
 * The maximum number of command inputs that there can be.
 */
#define MAX_INPUTS	32

/**
 * The maximum length of a command input name.
 */
#define CMD_INPUT_LEN	32

/**
 * The global list of command inputs.
 */
extern struct sc_input_table *sc_inputs;

/**
 * Describes the input string of a command.
 */
struct sc_input{
	char name[CMD_INPUT_LEN]; /**< Identifier name of the data. */
	int length;               /**< Max length of, including '\0'. */
};

/**
 * A table of command inputs and the number of inputs allocated.
 */
struct sc_input_table {
	int count;
	struct sc_input inputs[MAX_INPUTS];
};

/**
 * Converts from a string to an input value (int).
 * This is optimized for positive @c ints and operates on the string based on
 * its passed length, so the library @c atoi(3) could not be used.
 *
 * @param s       The string to convert
 * @param length  The length of @p s.
 *
 * @returns @p s represented as a positive @c int.
 * @returns @c -1 on error.
 */
int atoin(const char *s, size_t length);

/**
 * Allocates and zeros memory for a @c sc_input_table.
 *
 * @returns A malloc'd pointer to a @c sc_input_table.
 */
struct sc_input_table *alloc_input_table(void);

/**
 * Searches through the inputs table for the input with @p name.
 *
 * @param name	The name of the input to search for.
 *
 * @returns A pointer to a @c sc_input in the table.
 * @returns @c NULL if none were found.
 */
struct sc_input *name_to_input(const char *name);

/**
 * Returns a pointer to the last input in the table.
 *
 * @returns A pointer to the last @sc_input in the table.
 */
struct sc_input *cur_input(void);

#endif /* INPUT_H */

