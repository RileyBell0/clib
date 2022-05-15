#ifndef CLIB_STD_STRUC_CIRCULAR_ARRAY_H
#define CLIB_STD_STRUC_CIRCULAR_ARRAY_H

#include "../memory.h"
#include <stdbool.h>

typedef struct circular_array_t {
    size_t elem_size;
    void* data;
    int len;
    int capacity;
    int first;
    int end;
    bool destroy_on_remove;
    void (*destroy)(void* data);
} circular_array_t;

circular_array_t circular_array_new(size_t elem_size);

// add
// at a certain position, at the start, at the end
void circular_array_append(circular_array_t* arr, void* elem);
void circular_array_prepend(circular_array_t* arr, void* elem);
void circular_array_insert(circular_array_t* arr, int index, void* elem);
void circular_array_set(circular_array_t* arr, int index, void* elem);

// get
void* circular_array_index(circular_array_t* arr, int index);

// remove
void circular_array_remove_at(circular_array_t* arr, int index);
bool circular_array_remove(circular_array_t*, void* data);
void circular_array_pop(circular_array_t* arr, void* dest, int index);



// resize

#endif