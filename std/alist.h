#ifndef CLIB_STD_ALIST_H
#define CLIB_STD_ALIST_H

#include <stdint.h>
#include <stdarg.h>
#include "array.h"

#define ALIST_REALLOC_MULTIPLIER 2
#define ALIST_NULL UINT32_MAX
#define ALIST_SAFE_DESTROY

/*
 * All of these lists use UINT32_MAX as a NULL value
 * but might consider changing this later
*/

typedef struct alist_node_t
{
    uint32_t next;
    uint32_t prev;
} alist_node_t;

typedef struct alist_t
{
    void* list_start;
    uint32_t first;
    uint32_t last;
    uint32_t size;
    uint32_t capacity;
    size_t element_size;
    size_t block_size;
    int (*compare)(const void *first, const void *second);
    void (*destroy)(void *data);
} alist_t;

/*
 * Returns an empty alist - no memory allocated
*/
alist_t new_alist(size_t element_size);

/*
 * Appends the given item to the list, extending the list if neccessary
*/
void alist_append(alist_t *list, void* data);

/*
 * Gets the element in the alist at the given position
*/
void* alist_get_element(alist_t* list, uint32_t element);

/*
 * Given a node in an alist, removes it
*/
void alist_remove_node(alist_t *list, alist_node_t* node, uint32_t curr);

/*
 * Removes the first element in the alist matching the provided element
*/
int alist_remove(alist_t* list, void* element);

#endif
