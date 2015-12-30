/**
 * @file parse.h
 * @author Alex Striff
 *
 * @brief Parses the command (and input) specification YAML file, and generates
 * the global datastructures used to access the commands and their inputs.
 */

#ifndef PARSE_H
#define PARSE_H

#ifndef _BSD_SOURCE /* For snprintf(3). */
#define _BSD_SOURCE
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <yaml.h>

#include "dirs.h"
#include "input.h"
#include "command.h"
#include "yaml_macros.h"

#ifndef COMMAND_LIB
#define COMMAND_LIB	NULL
#endif /* COMMAND_LIB */

/**
 * Internal dlsym handle for command function symbols.
 */
void *sc_handle;

/**
 * The maximum number of command inputs that there can be.
 */
#define MAX_INPUTS		32

/**
 * The maximum length of a command input name.
 */
#define CMD_INPUT_LEN	32
/**
 * The "root" scalar that specifies inputs.
 */
#define ROOT_INPUTS_SCALAR		"inputs"

/**
 * The "root" scalar that specifies commands.
 */
#define ROOT_COMMANDS_SCALAR	"commands"

/**
 * The "attribute" scalar that specifies the command parent.
 */
#define PARENT_SCALAR			"parent"
/**
 * The "attribute" scalar that specifies the command delimiter.
 */
#define DELIMITER_SCALAR		"delimiter"
/**
 * The "attribute" scalar that specifies the command function.
 */
#define FUNCTION_SCALAR			"function"
/**
 * The "attribute" scalar that specifies the command input.
 */
#define INPUT_SCALAR			"input"

/**
 * Parses a command file.
 *
 * From the file, the global sc_inputs and sc_commands datastructures will be
 * populated. They must be freed later with @c free_command_info().
 *
 * @param file    The path of the file to parse. Often @c COMMAND_FILE.
 * @param logfile File to log parsing in iformation to.
 *
 * @returns @c 0 on success and a positive integer status code upon error
 * (also logs the error message to @p logfile).
 */
int parse_command_file(const char *file, FILE *logfile);

/**
 * Frees all command-related memory.
 */
void free_command_info(void);

/**
 * Tests input and command parsing, logging results to @p logfile.
 *
 * @param logfile The file to log notes and the ongoing status of the test to.
 *
 * @retval 1 if test succeeded.
 * @retval 0 if test failed.
 */
int test_sc_parse(FILE *logfile);

#endif /* PARSE_H */

