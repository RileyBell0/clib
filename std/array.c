#include "array.h"

static int dynamic_array_extend(dynamicArray_t *base);

void array_set_element(void *array, void *data, unsigned int element, unsigned int elementSize)
{
    memcpy(((char *)array + (elementSize * element)), data, elementSize);
}

void *array_get_element(void *array, unsigned int element, unsigned int elementSize)
{
    return (void *)((char *)array + (element * elementSize));
}

array_t new_array(unsigned int elements, unsigned int element_size)
{
    array_t newArray;

    if (elements != 0)
    {
        newArray.dat = safe_malloc(elements * element_size);
    }
    else
    {
        newArray.dat = NULL;
    }

    newArray.len = elements;

    return newArray;
}

int array_extend(array_t *base, unsigned int elementSize, unsigned int extraSpace)
{
    if (base == NULL)
    {
        return FALSE;
    }

    // Try and resize to an array with the extra space
    array_resize(base, elementSize, base->len + extraSpace);

    return TRUE;
}

int array_resize(array_t *base, unsigned int elementSize, unsigned int newLength)
{
    // cannot resize to be smaller
    if (base == NULL || base->len >= newLength)
    {
        return FALSE;
    }

    // Resize
    base->dat = realloc(base->dat, elementSize * newLength);

    // Updating the size of the array
    base->len = newLength;

    // Assert the resizing worked
    assert(base->dat);

    return TRUE;
}

dynamicArray_t new_dynamic_array(unsigned int element_size)
{
    dynamicArray_t newArray = {0};

    newArray.elementSize = element_size;

    return newArray;
}

int dynamic_array_append(dynamicArray_t *base, void *element)
{
    if (base == NULL)
    {
        return FALSE;
    }

    // Extending the array if needed
    if (base->maxLen == base->len)
    {
        dynamic_array_extend(base);
    }

    // Copies data from the address 'element' to the array
    memcpy(&((char *)base->dat)[(base->len * base->elementSize)], element, base->elementSize);

    base->len += 1;

    return TRUE;
}

// Extends an array (if it is full)
static int dynamic_array_extend(dynamicArray_t *base)
{
    if (base == NULL || base->len < base->maxLen)
    {
        return FALSE;
    }

    unsigned int newLen;

    // Add one to the length if its less than 3 (after which point dividing makes sense)
    if (base->maxLen <= 3)
    {
        newLen = (base->maxLen + 1);
    }
    else
    {
        // 1.5x extension factor (maxLen + maxLen/2)
        newLen = base->maxLen + (base->maxLen / 2);
    }

    // Extend the array
    base->dat = realloc(base->dat, newLen * base->elementSize);
    if (base->dat == NULL)
    {
        return FALSE;
    }

    base->maxLen = newLen;

    return TRUE;
}

int dynamic_array_safe_resize(dynamicArray_t *array, unsigned int newLen)
{
    // If less than the current array length - FAIL
    if (newLen <= array->len)
    {
        return FALSE;
    }

    // If greater than the current array length but less than or equal
    // to the current maximum length of the array
    if (newLen <= array->maxLen)
    {
        return TRUE;
    }

    // Resize the array
    array->dat = realloc(array->dat, array->elementSize * newLen);
    assert(array->dat);

    // Update the length of the array
    array->len = newLen;
    array->maxLen = newLen;

    return TRUE;
}

void dynamic_array_set_element(dynamicArray_t *array, unsigned int element, void *data)
{
    // Gets a pointer to the start of the required element
    if (array->maxLen < element)
    {
        // If the length of the array when extended is less than
        // the required length to place the new element at
        if (element > array->maxLen + (array->maxLen / 2))
        {
            dynamic_array_safe_resize(array, element);
        }
        else
        {
            dynamic_array_extend(array);
        }
    }

    // Update the length of the array (if the element was placed past the current length)
    if (element > array->len)
    {
        array->len = element;
    }

    // Set the element in the array at the given position to the data recieved
    memcpy((((char *)array->dat) + (array->elementSize * element)), data, array->elementSize);
}

void *dynamic_array_get_element(dynamicArray_t *array, unsigned int element)
{
    // If the required element is outside the bounds of the array
    if (element > array->len)
    {
        return NULL;
    }

    // Return a pointer to the start of the element in the array
    // by using pointer arithmetic to shift the pointer along the array
    return (void *)((char *)array->dat + (element * array->elementSize));
}

// Wrapper function to destroy the contents of an array
void array_destroy(array_t toDestroy)
{
    destroy(toDestroy.dat);
}

// Wrapper function to destroy the contents of an array
void dynamic_array_destroy(dynamicArray_t toDestroy)
{
    destroy(toDestroy.dat);
}