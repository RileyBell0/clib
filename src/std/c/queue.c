#include "../queue.h"

queue_t queue_new(size_t elem_size, int capacity) {
  return circular_new(elem_size, capacity);
}

void queue_enqueue(queue_t *queue, void *data) {
  circular_append((circular_t *)queue, data);
}

bool queue_dequeue(queue_t *queue, void *dest) {
  return circular_pop_first((circular_t *)queue, dest);
}

void queue_destroy(queue_t *queue) { circular_destroy((circular_t *)queue); }
