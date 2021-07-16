#ifndef CLIB_STD_POINTER_H
#define CLIB_STD_POINTER_H

#include <stdlib.h>

// TODO confirm that either of these functions work

void* pointer_offset(void* to_offset, int blocks, size_t block_size);

int pointer_count_blocks(void* start, void* block_start, size_t block_size);

intptr_t pointer_difference(void* start, void* end);

#endif