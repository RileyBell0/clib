#include "array.h"

static int dynamic_array_extend(dynamicArray *base);

array new_array(unsigned int elements, unsigned int element_size)
{
    array newArray;

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

int array_extend(array *base, unsigned int elementSize, unsigned int extraSpace)
{
    if (base == NULL)
    {
        return FALSE;
    }

    // Try and resize to an array with the extra space
    array_resize(base, elementSize, base->len + extraSpace);

    return TRUE;
}

int array_resize(array *base, unsigned int elementSize, unsigned int newLength)
{
    // cannot resize to be smaller
    if (base == NULL || base->len >= newLength)
    {
        return FALSE;
    }

    // Resize
    base->dat = realloc(base->dat, elementSize*newLength);

    // Updating the size of the array
    base->len = newLength;

    // Assert the resizing worked
    assert(base->dat);

    return TRUE;
}


dynamicArray new_dynamic_array(unsigned int element_size)
{
    dynamicArray newArray;

    newArray.dat = NULL;
    
    newArray.maxLen = 0;
    newArray.len = 0;

    newArray.elementSize = element_size;

    return newArray;
}

int dynamic_array_append(dynamicArray *base, void* element)
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
    memcpy(&((char*)base->dat)[(base->len * base->elementSize)], element, base->elementSize);

    base->len += 1;

    return TRUE;
}

// Extends an array (if it is full)
static int dynamic_array_extend(dynamicArray *base)
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
        newLen = base->maxLen + (base->maxLen/2);
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

// Wrapper function to destroy the contents of an array
void array_destroy(array toDestroy)
{
    destroy(toDestroy.dat);
}