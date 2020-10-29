/*
 * Author:          Riley Bell
 * Creation Date:   29/10/2020
*/

#ifndef CLIB_STD_STACK_H
#define CLIB_STD_STACK_H

#include "list.h"

typedef struct list_t stack_t;

/*
 * Creates and returns a new empty stack_t
*/
stack_t new_stack(unsigned int elementSize);

/*
 * Adds the given data to the top of the given stack
*/
void stack_push(stack_t* stack, void *data);

/*
 * Removes and returns a pointer to the top element in the stack
*/
void* stack_pop(stack_t* stack);

/*
 * Returns a pointer to the top element in the stack
*/
void* stack_top(stack_t* stack);

#endif