#include "../pointer.h"

void* pointer_offset(void* to_offset, int blocks, size_t block_size) {
  intptr_t offset = blocks * block_size;

  // Add 'offset' chars to the pointer and return
  return (void *)(((char *)to_offset) + offset);
}

int pointer_count_blocks(void* start, void* block_start, size_t block_size) {
  intptr_t difference = (intptr_t)start - (intptr_t)block_start;

  return difference / block_size;
}

intptr_t pointer_difference(void* start, void* end) {
  intptr_t difference = (intptr_t)end - (intptr_t)start;
  return difference;
}
