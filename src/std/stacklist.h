#ifndef CLIB_STD_STACKLIST_H
#define CLIB_STD_STACKLIST_H

#include "list.h"

typedef struct list_t stacklist_t;

/*
 * Creates and returns a new empty stacklist_t
 */
stacklist_t stacklist_new(size_t elementSize);

/*
 * Adds the given data to the top of the given stack
 */
void stacklist_push(stacklist_t *stack, void *data);

/*
 * Removes and returns a pointer to the top element in the stack
 */
void stacklist_pop(stacklist_t *stack, void* dest);

/*
 * Returns a pointer to the top element in the stack
 */
void *stacklist_top(stacklist_t *stack);

#endif