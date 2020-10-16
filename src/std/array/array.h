#ifndef ARRAY_H
#define ARRAY_H

#include "../general/general.h"
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

// Wrapper function to destroy the contents of an array
void array_destroy(array toDestroy);

int array_extend(array *base, unsigned int elementSize, unsigned int extraSpace);

int array_resize(array *base, unsigned int elementSize, unsigned int newLength);

// ----------- Dynamic Array -------------
dynamicArray new_dynamic_array(unsigned int element_size);

int dynamic_array_append(dynamicArray *base, void* element);

#endif