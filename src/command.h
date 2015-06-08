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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"
#include "element.h"

/**
 * Runs a command specfied in line.
 *
 * Not all commands will result in a token being generated.
 *
 * @param line The command string.
 * @param out The token result.
 * @param logfile The file to output of the command to.
 *
 * @retval 1
 * Success.
 * @note
 * In a case where the command does not produce a token, the token will be of
 * type @c NONE.
 *
 * @retval 0
 * In a case of a failed command that *should* produce a token, the token will
 * be of type @c NONE.
 */
int run_cmd(const char *line, sc_token *out, FILE *logfile);

#endif /* COMMAND_H */

