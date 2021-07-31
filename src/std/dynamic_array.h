#ifndef CLIB_STD_DYNAMIC_ARRAY_H
#define CLIB_STD_DYNAMIC_ARRAY_H

#include "array.h"
#include <stdbool.h>
#include <stdlib.h>

/*
 * Same as an array, but with append function with
 * automatic resizing. Setting elements in this array
 * is easier and doesnt require a cast since the array knows
 * the size of its stored elements
 */
typedef struct dynamic_array_t {
  size_t elem_size;
  void *data;
  unsigned int len;
  unsigned int capacity;
  bool was_allocated;
  bool destroy_on_remove;
  void (*destroy)(void *data);
} dynamic_array_t;

//////////////////////////////
// Initialisation
//////////////////////////////

/*
 * Returns a new empty dynamic array, with field destroy_on_remove set to true
 */
dynamic_array_t dynamic_array_new(size_t elem_size);

/*
 * Wraps the given data into a dynamic array. Will destroy the 'data' ptr if
 * the memory was allocated
*/
dynamic_array_t dynamic_array_wrap(void *data, int len, int capacity,
                                   size_t elem_size, bool was_allocated);

//////////////////////////////
// Basic Operations
//////////////////////////////

/*
 * Appends the data pointed to by 'element' to the dynamic array.
 */
void dynamic_array_append(dynamic_array_t *array, void *data);

/*
 * Inserts the data pointed to by 'element' to the dynamic
 * array at the specified index.
 * Supports negative indicies, where -1 means insert at the end (append)
 */
void dynamic_array_insert(dynamic_array_t *array, int index, void *data);

/*
 * Gets a pointer to the element at the given index in the array
 */
void *dynamic_array_get(dynamic_array_t *array, int index);

/*
 * Sets the value of the element in the array at the given index to the data
 * pointed to by 'data'
 */
void dynamic_array_set(dynamic_array_t *array, int index, void *data);

/*
 * Removes the first element which is deemed equal to 'elem' by the key function
 * where an equal value is represented by a return value of zero, or if no
 * key function is provided, removes the first element which has the same
 * value in memory as 'elem'
 */
bool dynamic_array_remove(dynamic_array_t *array, void *elem,
                          int (*key)(void *elem1, void *elem2));

/*
 * Removes the element at the given index in the dynamic array, shifting
 * elements past its position down
 */
void dynamic_array_remove_at(dynamic_array_t *array, int index);

/*
 * Returns a pointer to a copy of the element at the given index
 * ALLOCATES MEMORY - You are responsible for freeing the returned pointer
 * Supports negative indicies
 */
void *dynamic_array_copy_elem(dynamic_array_t *array, int index);

/*
 * Removes and returns a copy of the element at the given index from the array
 * ALLOCATES MEMORY - You are responsible for destroying it
 * Supports negative indicies
 */
void dynamic_array_pop(dynamic_array_t *array, void* dest, int index);

//////////////////////////////
// High Level functions
//////////////////////////////

/*
 * Sets the capacity of the array, cannot be lower than the array's len
 * and must be zero or a positive integer
 */
void dynamic_array_set_capacity(dynamic_array_t *array, int req_len);

/*
 * Extends the capacity to be of the new length specified
 * If the new length is lower than the current length, removes elements
 * beforehand, but does not lower capacity
 */
void dynamic_array_resize(dynamic_array_t *array, int new_len, void *template);

/*
 * Copies the contents of a dynamic array into a new array_t
 * with just enough space for all elements
 */
array_t dynamic_array_to_array(dynamic_array_t *array);

//////////////////////////////
// Cleanup
//////////////////////////////

/*
 * Given a dynamic array, removes all elements from it setting its length to
 * zero. If enabled, destroys the  data within each element with the supplied
 * destruction method;
 */
void dynamic_array_clear(dynamic_array_t *array);

/*
 * Destroys the data within the given dynamic array which was dynamically
 * allocated by other functions in this file
 */
void dynamic_array_destroy(dynamic_array_t *array);

#endif