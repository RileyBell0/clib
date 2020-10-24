/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
*/

#ifndef CLIB_STD_ARRAY_H
#define CLIB_STD_ARRAY_H

#include "general.h"
#include <string.h>

#define EXTENSION_FACTOR 1.5f

/*
 * Array struct which knows its length
 * data-type stored must be maintained and known by the user
 * 
 * can cast 
*/

typedef struct array_t
{
    void *dat;
    unsigned int len;
} array_t;

typedef struct dynamicArray_t
{
    void *dat;
    unsigned int len;
    unsigned int maxLen;
    unsigned int elementSize;
} dynamicArray_t;

// ----------- Array -------------
// returns a new array with the length set to 'elements'
array_t new_array(unsigned int elements, unsigned int element_size);

// generic array element setting method
void array_set_element(void *array, void *data, unsigned int element, unsigned int elementSize);
// generic array get ptr to element
void *array_get_element(void *array, unsigned int element, unsigned int elementSize);

// Wrapper function to destroy the contents of an array
void array_destroy(array_t toDestroy);
void dynamic_array_destroy(dynamicArray_t toDestroy);

int array_extend(array_t *base, unsigned int elementSize, unsigned int extraSpace);

int array_resize(array_t *base, unsigned int elementSize, unsigned int newLength);

// ----------- Dynamic Array -------------
dynamicArray_t new_dynamic_array(unsigned int element_size);

int dynamic_array_append(dynamicArray_t *base, void *element);

#endif