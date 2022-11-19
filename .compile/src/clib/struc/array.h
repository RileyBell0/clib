#ifndef CLIB_STD_STRUC_ARRAY_H
#define CLIB_STD_STRUC_ARRAY_H

#include "../sys/error.h"
#include "../memory.h"
#include "../index.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/*
 * Array struct which knows its length
 * data-type stored must be maintained and known by the user
 */
typedef struct array_t
{
  void *data;
  size_t len;
  void (*destroy)(void *data);
  size_t element_size;
} array_t;

//////////////////////////////
// Initialisation
//////////////////////////////

/*
 * returns a new array with the length set to 'elements'
 */
array_t array_new(size_t len, size_t element_size, void (*destroy)(void *data));

//////////////////////////////
// Basic Operations
//////////////////////////////

/*
 * Gets a pointer to the element at the given index in
 * the array provided
 * Negative Indexes are Supported
 */
void *array_get(array_t *array, size_t index);

/*
 * Sets the given index in the array to the data provided
 * Negative Indexes are Supported
 */
void array_set(array_t *array, size_t index, void *data);

//////////////////////////////
// Generic Arrays
//////////////////////////////

/*
 * Method to set a given element from an array of an unknown type
 * Intended for use with array structures that know their element size
 */
void array_generic_set(void *array, void *elem, size_t index,
                       size_t element_size);

/*
 * Method to get a given element from an array of an unknown type
 * Intended for use with array structures that know their element size
 */
void *array_generic_get(void *array, size_t index,
                        size_t element_size);

//////////////////////////////
// Cleanup
//////////////////////////////

/*
 * Destroys the data within the given array which was dynamically allocated by
 * other functions in this file
 */
void array_destroy(array_t *array);

#endif