/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
 */

#ifndef CLIB_STD_ARRAY_H
#define CLIB_STD_ARRAY_H

#include "memory.h"
#include <stdint.h>
#include <string.h>

/*
 * Ammount to extend a dynamic array whenever an element
 * is appended to a full array
 */
#define EXTENSION_FACTOR 1.5f

/*
 * Array struct which knows its length
 * data-type stored must be maintained and known by the user
 */
typedef struct array_t {
  void *data;
  unsigned int len;
  size_t element_size;
} array_t;

/*
 * Same as an array, but with append function with
 * automatic resizing. Setting elements in this array
 * is easier and doesnt require a cast since the array knows
 * the size of its stored elements
 */
typedef struct dynamic_array_t {
  void *data;
  unsigned int len;
  unsigned int capacity;
  size_t element_size;
} dynamic_array_t;

// ----------- Array -------------
/*
 * returns a new array with the length set to 'elements'
 */
array_t new_array(unsigned int elements, size_t element_size);

/*
 * Given a pointer to an existing array, wraps it into an array_t
 */
array_t array_wrap(void *array, size_t element_size, unsigned int len);

/*
 * Gets a pointer to the element at the given index in
 * the array provided
 */
void *array_index(array_t *array, unsigned int index);

/*
 * Sets the given index in the array to the data provided
 */
void array_set_index(array_t *array, unsigned int index, void *data);

/*
 * Method to set a given element from an array of an unknown type
 * Intended for use with array structures that know their element size
 */
void array_set_element(void *array, void *data, unsigned int element,
                       size_t element_size);

/*
 * Method to get a given element from an array of an unknown type
 * Intended for use with array structures that know their element size
 */
void *array_get_element(void *array, unsigned int element, size_t element_size);

/*
 * Resizes the array to the given length. Must be larger than the current length
 */
int array_resize(array_t *array, unsigned int newLength);

// ----------- Dynamic Array -------------
/*
 * Returns a new empty dynamic array
 */
dynamic_array_t new_dynamic_array(size_t element_size);

/*
 * Attempts to add the data pointed to by 'element' to the given dynamic
 * array. Returns TRUE on success, FALSE on failure (if base or element were
 * NULL)
 */
int dynamic_array_append(dynamic_array_t *base, void *element);

/*
 * If neccesary, extends the array to be exactly the given length
 * Returns FALSE if the new length is less than the current length
 * Does nothing if the maximum length is greater than the required
 * new length
 */
int dynamic_array_safe_resize(dynamic_array_t *array, unsigned int newLen);

/*
 * Sets the value of the element in the array at the given index
 */
void dynamic_array_set_element(dynamic_array_t *array, unsigned int element,
                               void *data);

/*
 * Gets a pointer to the element at the given index in the array
 */
void *dynamic_array_get_element(dynamic_array_t *array, unsigned int element);

/*
 * Copies the contents of a dynamic array into a new array_t
 * with just enough space for all elements
 */
array_t dynamic_array_to_array(dynamic_array_t *array);

/*
 * Destroys the data within the given array which was dynamically allocated by
 * other functions in this file
 */
void array_destroy(array_t toDestroy);

/*
 * Destroys the data within the given dynamic array which was dynamically
 * allocated by other functions in this file
 */
void dynamic_array_destroy(dynamic_array_t toDestroy);

#endif