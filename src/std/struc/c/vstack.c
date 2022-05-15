#include "../vstack.h"

vstack_t vstack_new(size_t elem_size) {
  return vector_new(elem_size);
}

void vstack_push(vstack_t *stack, void *data) {
  vector_append((vector_t *)stack, data);
}

bool vstack_pop(vstack_t *stack, void *dest) {
  if (stack->len > 0) {
    vector_pop((vector_t *)stack, dest, -1);
    return true;
  }
  return false;
}

void *vstack_top(vstack_t *stack) {
  return vector_get((vector_t *)stack, -1);
}
