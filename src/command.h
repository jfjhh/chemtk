/**
 * @file command.h
 * @author Alex Striff
 *
 * @brief Runs commands that produce numbers to calculate with.
 *
 * An example of a command's output would be the atomic mass of an element from
 * the periodic table.
 *
 * @todo Write down the specs for @c COMMAND_FILE.
 * @todo Document all available commands.
 */

#ifndef COMMAND_H
#define COMMAND_H

/**
 * The maximum length of a command string, including the terminating @c NULL
 * byte.
 */
#define CMD_STRLEN	256

/**
 * Maximum length of a command "long" name identifier string.
 */
#define CMD_NAME_LEN	48

/**
 * The maximum number of command children that there can be.
 */
#define CMD_CHILDREN	32

/**
 * The maximum theoretical command depth.
 */
#define MAX_CHILD_DEPTH	32

/**
 * Max lines in a command tree file.
 */
#define CMD_MAX_LINES	1024

/**
 * Character used to delimit command depth (nesting / children in the tree).
 */
#define CMD_DEPTH_DELIM	'\t'

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <string.h>
#include <ctype.h>

#include "dirs.h"
#include "token.h"
#include "stack.h"
#include "element.h"
#include "constant.h"
#include "input.h"
#include "commands/command_interface.h"

/**
 * The global command tree.
 */
extern struct sc_command_tree *sc_commands;

/**
 * An entry in the command tree.
 */
struct sc_command_entry {
	char delimiter;          /**< A single character delimiter. */
	char name[CMD_NAME_LEN]; /**< The "long" name of the command entry. */
	struct sc_input *input;  /**< An input in the global table. */
	sc_command_func  func;   /**< The command function to run. */
};

/**
 * Command (parse) tree.
 */
struct sc_command_tree {
	int num_children; /**< The number of children in use. */
	int depth;        /**< The depth of the tree from the root. */
	struct sc_command_entry *entry;                  /**< The node's entry. */
	struct sc_command_tree  *children[CMD_CHILDREN]; /**< The children. */
};

/**
 * Allocates a command tree, and initializes values to @c NULL -like values.
 * 
 * @returns a pointer to a @c sc_command_tree, or @c NULL on error.
 */
struct sc_command_tree *alloc_command_tree(void);

/**
 * Frees the global command tree from memory.
 */
void free_command_tree(struct sc_command_tree *root);

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
int is_sc_command(char *line);

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
int run_sc_command(char *line, sc_token *out, FILE *logfile);

/**
 * Creates a command entry from the given data.
 *
 * @param delimiter A single character delimiter.
 * @param name      The "long" name of the command entry.
 * @param func      The command function to run.
 * @param input     An input in the global table.
 * @param handle    Handle returned from @c dlopen(NULL, RTLD_LAZY).
 *
 * @returns An entry with the specified data, or @c NULL on error.
 */
struct sc_command_entry *create_command_entry(char delimiter,
		const char *name, const char *func, const char *input, void *handle);

/**
 * Encapsulates an @p entry into a new tree, and then makes that new tree a
 * child of @p tree.
 *
 * @param entry The entry to add to the tree.
 * @param tree  The parent tree of the entry.
 *
 * @returns The new tree or @c NULL on error.
 */
struct sc_command_tree *add_command_entry(struct sc_command_entry *entry,
		struct sc_command_tree *tree);

/**
 * Matches the long name of an entry with the entry's parent tree.
 *
 * @param name The name to search for.
 * @param tree The tree to start searching in.
 */
struct sc_command_tree *name_to_sc_parent_tree(const char *name);

/**
 * Prints the sc_commands tree to @c stderr.
 */
void print_sc_commands(FILE *file);

/**
 * Tests command @c operate() routine, logging results to @p logfile.
 *
 * @param logfile The file to log notes and the ongoing status of the test to.
 *
 * @retval 1 if test succeeded.
 * @retval 0 if test failed.
 */
int test_sc_command(FILE *logfile);

#endif /* COMMAND_H */

