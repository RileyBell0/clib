#ifndef CLIB_LIST_H
#define CLIB_LIST_H

#include <stdlib.h>
#include <string.h>
#include "../general/general.h"
#include "../bool/bool.h"

#define ERROR -1

// Struct for storing a single noede
typedef struct list_node
{
    void *data;
    struct list_node *next;
    struct list_node *prev;
} list_node;

// Struct for storing a single list
typedef struct list
{
    list_node *first_node;
    list_node *last_node;
    unsigned int size;
    unsigned int elementSize;
} list;

// Returns a new list where the stored elements will be of size (elementSize)
list new_list(unsigned int elementSize);

// Adds the data to the end of the list
// Requires a pointer to the element that is going to be added (element data gets copied in)
void list_append(list *list, void *data);

// Finds and returns the node at the given index
// If the index is invalid, exits with code (-1)
void *list_remove_at(list *list, int index);

/*
 * USAGE:
 *  Requires
 *      - List in which the item is contained
 *      - A pointer to the item
*/
void *list_remove_element(list *list, void *toRemove);

void* list_remove_node(list_node *toRemove);

// frees all dynamically allocated data in a list.
// IMPORTANT: Send a 'delete_data' function IF your data cannot be freed just by calling free() on it's pointer
void list_destroy(list *list, void (*delete_data)(void *data));

#endif