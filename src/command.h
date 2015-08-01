/**
 * @file command.h
 * @author Alex Striff
 *
 * @brief Runs commands that produce numbers to calculate with.
 *
 * An example of a command's output would be the atomic mass of an element from
 * the periodic table.
 *
 * @todo Write the doc file for what commands do and do not exist.
 */

#ifndef COMMAND_H
#define COMMAND_H

/**
 * The maximum length of a command string, including the terminating @c NULL
 * byte.
 */
#define CMD_STRLEN	64

/**
 * The maximum number of command inputs that there can be.
 */
#define CMD_INPUTS	32

/**
 * Maximum length of a command input identifier string.
 */
#define CMD_NAME_LEN	32

/**
 * The maximum number of command children that there can be.
 */
#define CMD_CHILDREN	32

/**
 * Character used to delimit command depth (nesting / children in the tree).
 */
#define CMD_DEPTH_DELIM	'\t'

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <string.h>
#include <ctype.h>

#include "dirs.h"
#include "token.h"
#include "stack.h"
#include "element.h"
#include "constant.h"

/**
 * The global command tree.
 */
extern struct sc_command_tree *sc_commands;

/**
 * The global list of command inputs.
 */
extern struct sc_input_table *sc_inputs;

/**
 * Generic function pointer to run commands.
 *
 * Not all commands will result in a token being generated.
 *
 * @param line The command string.
 * @param out The token result.
 * @param logfile The file to output of the command to.
 *
 * @retval 1 Command succeeded.
 * @retval 0 Command failed.
 * @note
 * In a case where the command does not produce a token, the token will be of
 * type @c NONE.
 */
typedef int (*sc_command_func)(const char *line, sc_token *out, FILE *logfile);

/**
 * Describes the input string of a command.
 */
struct sc_command_input {
	char name[CMD_NAME_LEN]; /**< Identifier name of the data. */
	int length;              /**< Max length of, including '\0'. */
};

/**
 * A table of command inputs and the number of inputs allocated.
 */
struct sc_input_table {
	int count;
	struct sc_command_input inputs[CMD_INPUTS];
};

/**
 * An entry in the command tree.
 */
struct sc_command_entry {
	char token;              /**< A single character token. */
	char name[CMD_NAME_LEN]; /**< The "long" name of the token. */
	struct sc_command_input *input; /**< An input in the global table. */
	sc_command_func         func;   /**< The command function to run. */
};

/**
 * Command (parse) tree.
 */
struct sc_command_tree {
	int num_children;                  /**< The number of children in use. */
	struct sc_command_entry *entry;    /**< The node's entry. */
	struct sc_command_tree  *children; /**< Parse tree children. */
};

/**
 * Creates the command lookup table in memory.
 *
 * @retval 0 Could not create the command table.
 * @retval 1 Success.
 */
int init_sc_commands(void);

/**
 * Frees the command lookup table from memory.
 */
void free_sc_commands(void);

/**
 * Checks if a string is a valid command.
 *
 * @param line The command string.
 *
 * @retval 0
 * @p line is not a command.
 * @retval 1
 * @p line is a command.
 */
int is_sc_command(const char *line);

/**
 * Runs a command specfied in line.
 *
 * Assumes that @p line was previously verified to be a valid
 * @c sc_command_type.
 *
 * In a case where the command does not produce a token, the token will be of
 * type @c NONE.
 *
 * @param line The command string.
 * @param out The token result.
 * @param logfile The file to output of the command to.
 *
 * @retval 1 Command succeeded.
 * @retval 0 Command failed.
 */
int run_sc_command(const char *line, sc_token *out, FILE *logfile);

/**
 * Tests command @c operate() routine, logging results to @p logfile.
 *
 * @param logfile The file to log notes and the ongoing status of the test to.
 *
 * @retval 1 if test succeeded.
 * @retval 0 if test failed.
 *
 * @todo Write test_command().
 */
int test_sc_command(FILE *logfile);

#endif /* COMMAND_H */

