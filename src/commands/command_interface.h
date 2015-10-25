#ifndef COMMAND_INTERFACE_H
#define COMMAND_INTERFACE_H

#include <stdio.h>

#include "token_interface.h"

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
typedef int (*sc_command_func)(char *line, sc_token *out, FILE *logfile);

#endif /* COMMAND_INTERFACE_H */

