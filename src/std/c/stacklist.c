#include "../stacklist.h"

// TODO make sure each 'pop' function takes a dest argument, so we dont have to
// allocate memory
stacklist_t stacklist_new(size_t elementSize) {
  return list_new(elementSize);
}

void stacklist_push(stacklist_t *stack, void *data) {
  list_append((list_t *)stack, data);
}

void stacklist_pop(stacklist_t *stack, void* dest) {
  return list_pop((list_t *)stack, dest, -1);
}

void *stacklist_top(stacklist_t *stack) {
  return list_get((list_t *)stack, -1);
}
