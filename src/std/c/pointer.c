#include "../pointer.h"

void* pointer_offset(void* to_offset, int blocks, size_t block_size) {
  ptr_int_t offset = blocks * block_size;

  // Add 'offset' chars to the pointer and return
  return (void *)(((char *)to_offset) + offset);
}

int pointer_count_blocks(void* start, void* block_start, size_t block_size) {
  ptr_int_t difference = (ptr_int_t)start - (ptr_int_t)block_start;

  return difference / block_size;
}

ptr_int_t pointer_difference(void* start, void* end) {
  ptr_int_t difference = (ptr_int_t)end - (ptr_int_t)start;
  return difference;
}
