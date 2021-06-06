/*
 * Author:          Riley Bell
 * Creation Date:   29/10/2020
 */

#ifndef CLIB_STD_STACK_H
#define CLIB_STD_STACK_H

#include "list.h"

typedef struct list_t stacklist_t;

/*
 * Creates and returns a new empty stacklist_t
 */
stacklist_t new_stack(unsigned int elementSize);

/*
 * Adds the given data to the top of the given stack
 */
void stack_push(stacklist_t *stack, void *data);

/*
 * Removes and returns a pointer to the top element in the stack
 */
void *stack_pop(stacklist_t *stack);

/*
 * Returns a pointer to the top element in the stack
 */
void *stacklist_top(stacklist_t *stack);

#endif