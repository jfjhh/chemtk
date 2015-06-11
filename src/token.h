/**
 * @file token.h
 * @author Alex Striff
 *
 * @brief Provides a generic stack member datatype: a parsed token.
 *
 * @todo Make accessor macros work with pointers. E.g.:
 * @code
 * #define SC_FLT(A)	A->data.value.flt
 * @endcode
 */

#ifndef TOKEN_H
#define TOKEN_H

/* Forward declarations. */
typedef struct sc_token sc_token;
struct sc_token_value;
union sc_token_data;

/**
 * Keeps track of the token's current type.
 */
enum sc_token_type {
	NONE = 0,  /**< Error or uninitialized type. */
	VALUE,     /**< Generic numeric value type. */
	CMD,       /**< A string that represents a command to be evaluated. */
	OPERATOR,  /**< A stack operator. */
};

/**
 * @def SCT_FLT(A)
 * @brief Access a sc_token's floating point data.
 *
 * @param A The sc_token to access.
 */
#define SCT_FLT(A)	A->data.value.flt

/**
 * @def SCT_FIG(A, B)
 * @brief Access a sc_token's sig. fig. data.
 *
 * @param A The sc_token to access.
 * @param B The sig_fig_field to access.
 */
#define SCT_FIG(A, B)	A->data.value.figs[B]

/**
 * @def SCT_FIGS(A)
 * @brief Access a sc_token's <tt>significant figure array</tt>.
 *
 * @param A The sc_token to access.
 */
#define SCT_FIGS(A)	A->data.value.figs

/**
 * @def SCT_BOOL(A)
 * @brief Access a sc_token's boolean value data.
 *
 * @param A The sc_token to access.
 */
#define SCT_BOOL(A)	A->data.value.boolean

/**
 * @def SCT_CMD(A)
 * @brief Access a sc_token's command string.
 *
 * @param A The sc_token to access.
 */
#define SCT_CMD(A)	A->data.cmd

/**
 * @def SCT_OP(A)
 * @brief Access a sc_token's operator char.
 *
 * @param A The sc_token to access.
 */
#define SCT_OP(A)	A->data.op

/**
 * @def SCT_TYPE(A)
 * @brief Access a sc_token's type.
 *
 * @param A The sc_token to access.
 */
#define SCT_TYPE(A)	A->type

#include <stdio.h> /* DEBUGGING */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

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
	char           cmd[CMD_LINELEN];     /**< A string that represents a command
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
sc_token *sc_tokenize(const char *line);

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

