#ifndef CLIB_STD_STRUC_VECTOR_H
#define CLIB_STD_STRUC_VECTOR_H

#include "./array.h"
#include <stdbool.h>
#include <stdlib.h>

/*
 * Dynamically allocated array
 */
typedef struct vector_t
{
  void *data;
  size_t elem_size;
  size_t len;
  size_t capacity;
  void (*destroy)(void *data);
} vector_t;

//////////////////////////////
// Initialisation
//////////////////////////////

/*
 * Returns a new empty vector, with field destroy_on_remove set to true
 */
vector_t vector_new(size_t elem_size, void (*destroy)(void *destroy));

//////////////////////////////
// Basic Operations
//////////////////////////////

/*
 * Appends the data pointed to by 'element' to the vector.
 */
void vector_append(vector_t *array, void *data);

/*
 * Gets a pointer to the element at the given index in the array
 */
void *vector_get(vector_t *array, size_t index);

/*
 * Sets the value of the element in the array at the given index to the data
 * pointed to by 'data'
 */
void vector_set(vector_t *array, size_t index, void *data);

/*
 * Removes the first element which is deemed equal to 'elem' by the key function
 * where an equal value is represented by a return value of zero, or if no
 * key function is provided, removes the first element which has the same
 * value in memory as 'elem'
 */
bool vector_remove(vector_t *array, void *elem,
                   int (*compare)(void *elem1, void *elem2));

/*
 * Removes the element at the given index in the vector, shifting
 * elements past its position down
 */
void vector_remove_at(vector_t *array, size_t index);

/*
 * Removes the element from the array, and saves it to "dest"
 */
void vector_pop(vector_t *array, void *dest, size_t index);

/*
 * Copies vector data into an array
 */
array_t vector_to_array(vector_t *vec);

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