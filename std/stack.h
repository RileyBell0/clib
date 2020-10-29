#ifndef CLIB_STACK_H
#define CLIB_STACK_H

#include "list.h"

typedef struct list_t stack_t;

stack_t new_stack(unsigned int elementSize);

void stack_append(stack_t* stack, void *data);

void* stack_pop(stack_t* stack);

void* stacK_top(stack_t* stack);

#endif