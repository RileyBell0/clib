#include "../general.h"
#include <stdio.h>

void *safe_malloc(size_t size)
{
    void *newData = malloc(size);
    assert(newData);
    printf("\t- Malloc'd %zu bytes\n", size);
    return newData;
}

void *safe_calloc(size_t size)
{
    void *newData = calloc(1, size);
    assert(newData);
    printf("\t- Calloc'd %zu bytes\n", size);
    return newData;
}

void* safe_realloc(void* ptr, size_t size)
{
    void* result = realloc(ptr, size);
    assert(result);
    printf("\t- Realloc'd %zu bytes\n", size);
    return result;
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
    // if not-null;
    if (data)
    {
        void **data2 = (void **)data;
        destroy(*data2);
    }
}