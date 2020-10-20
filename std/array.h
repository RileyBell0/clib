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

typedef struct array {
    void* dat;
    unsigned int len;
} array;

typedef struct dynamicArray
{
    void* dat;
    unsigned int len;
    unsigned int maxLen;
    unsigned int elementSize;
} dynamicArray;

// ----------- Array -------------
// returns a new array with the length set to 'elements'
array new_array(unsigned int elements, unsigned int element_size);

void array_set_element(void* array, void* data, unsigned int element, unsigned int elementSize);
void *array_get_element(void* array, unsigned int element, unsigned int elementSize);

// Wrapper function to destroy the contents of an array
void array_destroy(array toDestroy);
void dynamic_array_destroy(dynamicArray toDestroy);

int array_extend(array *base, unsigned int elementSize, unsigned int extraSpace);

int array_resize(array *base, unsigned int elementSize, unsigned int newLength);

// ----------- Dynamic Array -------------
dynamicArray new_dynamic_array(unsigned int element_size);

int dynamic_array_append(dynamicArray *base, void* element);

#endif