#ifndef CLIB_STD_QUEUE_H
#define CLIB_STD_QUEUE_H

#include "circular_array.h"
#include <stdbool.h>

typedef struct circular_t queue_t;

queue_t queue_new(size_t elem_size, int capacity);

void queue_enqueue(queue_t* queue, void* data);

bool queue_dequeue(queue_t* queue, void* dest);

void queue_destroy(queue_t *queue);

#endif