#ifndef TOKEN_INTERFACE_H
#define TOKEN_INTERFACE_H

/* Forward declarations. */
typedef struct sc_token sc_token;
struct  sc_token_value;
union   sc_token_data;

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
 * The maximum length of a command string, including the terminating @c NULL
 * byte.
 */
#define CMD_STRLEN	256

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

#endif /* TOKEN_INTERFACE_H */

