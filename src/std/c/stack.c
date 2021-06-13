#include "../stack.h"

stacklist_t new_stack(unsigned int elementSize) {
  return new_list(elementSize);
}

void stack_push(stacklist_t *stack, void *data) {
  list_append((list_t *)stack, data);
}

void *stack_pop(stacklist_t *stack) {
  if (stack == NULL || stack->size == 0) {
    return NULL;
  }
  return list_pop((list_t *)stack, -1);
}

void *stacklist_top(stacklist_t *stack) {
  if (stack == NULL || stack->last_node == NULL) {
    return NULL;
  }

  return list_get((list_t *)stack, -1);
}
