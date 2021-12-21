#include "../stack.h"

stackk_t stack_new(size_t elem_size) {
  return dynamic_array_new(elem_size);
}

void stack_push(stackk_t *stack, void *data) {
  dynamic_array_append((dynamic_array_t *)stack, data);
}

bool stack_pop(stackk_t *stack, void *dest) {
  if (stack->len > 0) {
    dynamic_array_pop((dynamic_array_t *)stack, dest, -1);
    return true;
  }
  return false;
}

void *stack_top(stackk_t *stack) {
  return dynamic_array_get((dynamic_array_t *)stack, -1);
}
