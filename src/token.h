/**
 * @file token.h
 * @author Alex Striff
 *
 * @brief Provides a generic stack member datatype: a parsed token.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h> /* DEBUGGING */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "commands/token_interface.h"

#include "command.h"
#include "constant.h"
#include "ldmath.h"
#include "operator.h"

/**
 * Generic numeric value structure.
 */
struct sc_token_value {
	double flt;             /**< The number's value. */
	int figs[SIG_FIG_LEN];  /**< The significant figure array. */
	bool boolean;           /**< Boolean representation of the number. */
};

/**
 * Contains the token's data.
 */
union sc_token_data {
	struct sc_token_value value;         /**< A numeric value structure. */
	char           op;                   /**< A character operator. */
	char           cmd[CMD_STRLEN];      /**< A string that represents a command
										   to be evaluated. */
};

/**
 * A token is a generic stack member type that is able to represent any stack
 * object, including numeric values, commands, and operators.
 */
struct sc_token {
	enum sc_token_type type;
	union sc_token_data data;
};

/**
 * Parses @p line into a token.
 *
 * @param line The input string to be turned into a token.
 *
 * @retval sc_token *
 * A successfully parsed token.
 * @retval NULL
 * An error occured.
 *
 * @todo Write sc_tokenize().
 */
sc_token *sc_tokenize(char *line);

/**
 * Tests token routines, logging results to @p logfile.
 *
 * @param logfile The file to log notes and the ongoing status of the test to.
 *
 * @retval 1 if test succeeded.
 * @retval 0 if test failed.
 *
 * @todo Write test_sc_token().
 */
int test_sc_token(FILE *logfile);

#endif /* TOKEN_H */

