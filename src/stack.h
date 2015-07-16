/**
 * @file stack.h
 * @author Alex Striff
 *
 * @brief An implementation of a stack.
 */

#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Linked list structure of a stack member.
 */
typedef struct sc_stack {
	void            *data; /**< Pointer to the member's data. */
	struct sc_stack *next; /**< The next member down in the stack. */
} sc_stack;

/**
 * A function pointer that will print a member of a stack.
 * @anchor sc_stack_pfun
 *
 * @param data The data to be printed.
 * @param file The writable stream to print to.
 */
typedef void (*sc_stack_pfun)(void *data, FILE *file);

/**
 * Allocates memory for a new stack.
 *
 * @retval sc_stack *
 * A pointer to allocated memory for a stack.
 * @retval NULL
 * If memory allocation fails.
 */
sc_stack *new_sc_stack(void);

/**
 * Pushes @p data to @p stack.
 *
 * @param stack A pointer to the stack to be modified.
 * @param data The data to be pushed to the stack.
 */
void push_sc_stack(sc_stack **stack, void *data);

/**
 * Pops @p data from @p stack.
 *
 * @param stack A pointer to the stack to be modified.
 *
 * @retval void *
 * The data that was popped from the stack.
 * @retval NULL
 * The stack was @c NULL and could not be popped from.
 */
void *pop_sc_stack(sc_stack **stack);

/**
 * Completely deletes @p stack (and all associated data).
 *
 * @param stack A pointer to the stack to be deleted.
 * @param free_data A function pointer to the function that is used to free
 * the data of stack members. @c NULL can be used to do nothing.
 */
void delete_sc_stack(sc_stack *stack, void (*free_data)(void *data));

/**
 * Finds out how deep a stack is.
 *
 * @param stack The stack to investigate.
 *
 * @retval int The depth of the stack, @c 0 being a @c NULL stack.
 */
int sc_stack_depth(sc_stack *stack);

/**
 * Rotates a stack down.
 *
 * The top member of the stack becomes the bottom of the stack.
 *
 * @param stack The stack to operate on.
 */
void sc_stack_rotdown(sc_stack **stack);

/**
 * Rotates a stack up.
 *
 * The bottom member of the stack becomes the top of the stack.
 *
 * @param stack The stack to operate on.
 */
void sc_stack_rotup(sc_stack **stack);

/**
 * Prints a stack.
 *
 * @param file The writable stream that the stack will be printed to.
 * @param stack The stack to print.
 * @param pfun The function pointer that prints each member of the
 * stack.
 */
void print_sc_stack(FILE *file, sc_stack *stack,
		sc_stack_pfun pfun);

/**
 * A @link sc_stack_pfun @endlink
 * that prints a stack element that is a pointer.
 *
 * @param data The pointer to be printed.
 * @param file The writable stream to print to.
 */
void sc_print_ptr(void *data, FILE *file);

/**
 * A @link sc_stack_pfun @endlink
 * that prints a stack element that is a double.
 *
 * @param data The @c double to be printed.
 * @param file The writable stream to print to.
 */
void sc_print_double(void *data, FILE *file);

/**
 * Tests stack routines, logging results to @p logfile.
 *
 * @param logfile The file to log notes and the ongoing status of the test to.
 *
 * @retval 1 if test succeeded.
 * @retval 0 if test failed.
 */
int test_sc_stack(FILE *logfile);

#endif

