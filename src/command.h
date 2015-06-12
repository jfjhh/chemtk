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

#if ! defined(_XOPEN_SOURCE) || _XOPEN_SOURCE <= 700
#	define _XOPEN_SOURCE 700
#endif /* _XOPEN_SOURCE */

/**
 * The maximum length of a command string, including the terminating @c NULL
 * byte.
 */
#define CMD_LINELEN	64

/**
 * Denotes a type of command.
 *
 * Each member is a @c char that is used to delimit the command.
 * E.g. A command that starts with 'c' could be of @c cmd_type CONSTANT.
 */
enum sc_command_type {
	NOT_CMD      = 0,   /**< Invalid command (@c 0). */
	ELEMENT_CMD  = 'e', /**< An element command. */
	CONSTANT_CMD = 'c', /**< A constant command. */
	TEST_CMD     = 't', /**< A testing command. */
};

struct sc_command_entry;
struct sc_command_table;

/**
 * Table to hold all command entries.
 */
struct sc_command_table *sc_commands;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"
#include "element.h"
#include "constant.h"

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
 * type @c NOT_CMD.
 */
typedef int (*sc_command_fun)(const char *line, sc_token *token, FILE *logfile);

/**
 * Entry of a command for the lookup table.
 */
struct sc_command_entry {
	enum sc_command_type type;
	sc_command_fun *cmd_fun;
};

/**
 * All command entries.
 */
struct sc_command_table {
	struct sc_command_entry *entries;
	int count;
};

/**
 * Creates the command lookup table in memory.
 *
 * @retval 0 Failed to create the table.
 * @retval 1 Succeeded in creating the table.
 */
int init_sc_commands(void);

/**
 * Adds a command to the lookup table.
 *
 * @retval 0 Failed to add to the table.
 * @retval 1 Succeeded in adding the function to the table.
 */
int add_sc_command(sc_command_fun *cmd_fun, enum sc_command_type type);

/**
 * Frees the command lookup table from memory.
 */
void free_sc_commands(void);

/**
 * Checks if a string is a valid command.
 *
 * @param line The command string.
 *
 * @retval enum sc_command_type
 * The type of the command (nonzero), where @c NOT_CMD is not a command (@c 0).
 */
enum sc_command_type is_sc_command(const char *line);

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
int test_command(FILE *logfile);

#endif /* COMMAND_H */

