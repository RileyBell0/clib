#ifndef CLIB_STD_STRUC_VECTOR_H
#define CLIB_STD_STRUC_VECTOR_H

#include "array.h"
#include <stdbool.h>
#include <stdlib.h>

/*
 * Same as an array, but with append function with
 * automatic resizing. Setting elements in this array
 * is easier and doesnt require a cast since the array knows
 * the size of its stored elements
 */
typedef struct vector_t {
  size_t elem_size;
  void *data;
  int len;
  int capacity;
  bool was_allocated;
  bool destroy_on_remove;
  void (*destroy)(void *data);
} vector_t;

//////////////////////////////
// Initialisation
//////////////////////////////

/*
 * Returns a new empty vector, with field destroy_on_remove set to true
 */
vector_t vector_new(size_t elem_size);

/*
 * Wraps the given data into a vector. Will destroy the 'data' ptr if
 * the memory was allocated
*/
vector_t vector_wrap(void *data, int len, int capacity,
                                   size_t elem_size, bool was_allocated);

//////////////////////////////
// Basic Operations
//////////////////////////////

/*
 * Appends the data pointed to by 'element' to the vector.
 */
void vector_append(vector_t *array, void *data);

/*
 * Inserts the data pointed to by 'element' to the vector
 * at the specified index.
 * Supports negative indicies, where -1 means insert at the end (append)
 */
void vector_insert(vector_t *array, int index, void *data);

/*
 * Gets a pointer to the element at the given index in the array
 */
void *vector_get(vector_t *array, int index);

/*
 * Sets the value of the element in the array at the given index to the data
 * pointed to by 'data'
 */
void vector_set(vector_t *array, int index, void *data);

/*
 * Removes the first element which is deemed equal to 'elem' by the key function
 * where an equal value is represented by a return value of zero, or if no
 * key function is provided, removes the first element which has the same
 * value in memory as 'elem'
 */
bool vector_remove(vector_t *array, void *elem,
                          int (*key)(void *elem1, void *elem2));

/*
 * Removes the element at the given index in the vector, shifting
 * elements past its position down
 */
void vector_remove_at(vector_t *array, int index);

/*
 * Returns a pointer to a copy of the element at the given index
 * ALLOCATES MEMORY - You are responsible for freeing the returned pointer
 * Supports negative indicies
 */
void *vector_copy_elem(vector_t *array, int index);

/*
 * Removes and returns a copy of the element at the given index from the array
 * ALLOCATES MEMORY - You are responsible for destroying it
 * Supports negative indicies
 */
void vector_pop(vector_t *array, void* dest, int index);

//////////////////////////////
// High Level functions
//////////////////////////////

/*
 * Sets the capacity of the array, cannot be lower than the array's len
 * and must be zero or a positive integer
 */
void vector_set_capacity(vector_t *array, int req_len);

/*
 * Extends the capacity to be of the new length specified
 * If the new length is lower than the current length, removes elements
 * beforehand, but does not lower capacity
 */
void vector_resize(vector_t *array, int new_len, void *template);

/*
 * Copies the contents of a vector into a new array_t
 * with just enough space for all elements
 */
array_t vector_to_array(vector_t *array);

//////////////////////////////
// Cleanup
//////////////////////////////

/*
 * Given a vector, removes all elements from it setting its length to
 * zero. If enabled, destroys the  data within each element with the supplied
 * destruction method;
 */
void vector_clear(vector_t *array);

/*
 * Destroys the data within the given vector which was dynamically
 * allocated by other functions in this file
 */
void vector_destroy(vector_t *array);

#endif