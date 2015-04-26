#ifndef COMMAND_H
#define COMMAND_H

#if ! defined(_XOPEN_SOURCE) || _XOPEN_SOURCE <= 700
#	define _XOPEN_SOURCE 700
#endif /* _XOPEN_SOURCE */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"
#include "element.h"

/* Runs a command specfied in line.
 *
 * Not all commands will result in a token being generated.
 *
 * In a case where the command does not produce a token, the token will be of
 * type NONE, and run_cmd will return 1.
 *
 * In a case of a failed command that *should* produce a token, the token will
 * be of type NONE, and run_cmd will return 0.
 *
 * See a help or doc file for information on what commands exist.
 * TODO: (WRITE THE DOC FILE).
 */
int run_cmd(const char *line, sc_token *out, FILE *logfile);

#endif /* COMMAND_H */

