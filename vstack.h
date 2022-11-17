#ifndef CLIB_STD_STRUC_VSTACK_H
#define CLIB_STD_STRUC_VSTACK_H

#include "vector.h"
#include <stdbool.h>

typedef vector_t vstack_t;

vstack_t vstack_new(size_t elem_size);

vstack_t *vstack_push(vstack_t *stack, void *data);

bool vstack_pop(vstack_t *stack, void *dest);

void *vstack_top(vstack_t *stack);

#endif