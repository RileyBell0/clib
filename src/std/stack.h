#ifndef CLIB_STD_DYNAMICSTACK_H
#define CLIB_STD_DYNAMICSTACK_H

#include "dynamic_array.h"

typedef dynamic_array_t stackk_t;

stackk_t stack_new(size_t elem_size);

void stack_push(stackk_t *stack, void *data);

void stack_pop(stackk_t *stack, void *dest);

void *stack_top(stackk_t *stack);

#endif