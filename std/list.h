/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
*/

/* TODO
 * FUTURE RILEY - fix this
 * 
 * Undefined behaviour when
 * NULL values are recieved for any field
 * that is not a list_t* field
 * 
 * This functionality and checking
 * is yet to be implemented. not sure
 * whether to copy in a null value when
 * null is recieved or just to do nothing
 * and throw an error
*/

#ifndef CLIB_STD_LIST_H
#define CLIB_STD_LIST_H

#include <stdlib.h>
#include <string.h>
#include "general.h"
#include "bool.h"
#include "array.h"

#define ERROR -1

/*
 * Struct for storing a single noede
 */
typedef struct list_node_t
{
    void *data;
    struct list_node_t *next;
    struct list_node_t *prev;
} list_node_t;

/*
 * Struct for storing a single list
 */
typedef struct list_t
{
    list_node_t *first_node;
    list_node_t *last_node;
    unsigned int size;
    unsigned int elementSize;
} list_t;

/*
 * Converts the given list into an array
*/
array_t list_to_array(list_t list);

/*
 * Returns a new list where the stored elements will be of size (elementSize)
 */
list_t new_list(unsigned int elementSize);

/*
 * Adds the data to the end of the list
 * Requires a pointer to the element that is going to be added (element data gets copied in)
 */
void list_append(list_t *list, void *data);

/*
 * Finds and returns the node at the given index
 * If the index is invalid, exits with code (-1)
 */
void *list_remove_at(list_t *list, unsigned int index);

void *list_remove_first(list_t* list);
void *list_remove_last(list_t* list);

/*
 * USAGE:
 *  Requires
 *      - List in which the item is contained
 *      - A pointer to the item
 * removes the element at the given position in the list
 * if it can be found
*/
void *list_remove_element(list_t *list, void *toRemove);

/*
 * Removes the given node from the list, updates list size and pointers
 * accordingly
*/
void *list_remove_node(list_t *list, list_node_t *toRemove);

/*
 * Combines the two given lists, appending the list 'extension'
 * to the end of the list 'base'
*/
void list_combine(list_t *base, list_t *extension);

/*
 * frees all dynamically allocated data in a list.
 * IMPORTANT: Send a 'delete_data' function IF your data cannot be freed just by calling free() on it's pointer
 */
void list_destroy(list_t *list, void (*delete_data)(void *data));

#endif

// TODO
// should this also chcek if toRemove is non-null, or well, the data being recieved is not-null