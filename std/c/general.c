#include "../general.h"

void *safe_malloc(unsigned int size)
{
    void *newData = malloc(size);
    assert(newData);
    return newData;
}

void *safe_calloc(unsigned int size)
{
    void *newData = calloc(1, size);
    assert(newData);
    return newData;
}

void destroy(void *toDestroy)
{
    if (toDestroy)
    {
        free(toDestroy);
    }
}

void ptr_destroy(void *data)
{
    void **data2 = (void **)data;
    destroy(*data2);
}