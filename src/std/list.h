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

/*
 * TODO
 * What about if we make each alist keep a ledger alongside it so that
 * indexing is way faster?
 * so that if we ask for element number 12, it knows where that is kept
 * how would it keep track of this?
 * well whenever we're moving an element around, we need to know
 * what number element this is, so that we can move it about
 * so what if we make each element keep track of what element it is? thats
 * pointless, instead we can just keep a counter going  as we're going thoruhg
 * the list
 * so we need to make it so that resizing the list also resizes the ledger
 * or we can smack the ledger at the end of the list
 * wait no that doesnt work because, well it does it jsut means that whenever we
 * resize the list we have to resize the list, then copy the memory from the 
 * ledger's original location forward to wherever tf its new place is
 * but is that even possible? can you copy memory onto itself?
 * i dont imagine you can right?
 */

#ifndef CLIB_STD_LIST_H
#define CLIB_STD_LIST_H

#include "array.h"
#include <stdarg.h>
#include <stdint.h>

#define ERROR -1

/*
 * Struct for storing a single noede
 */
typedef struct list_node_t {
  void *data;
  struct list_node_t *next;
  struct list_node_t *prev;
} list_node_t;

/*
 * Struct for storing a single list
 */
typedef struct list_t {
  list_node_t *first_node;
  list_node_t *last_node;
  unsigned int size;
  size_t elementSize;
  int (*compare)(const void *first, const void *second);
} list_t;

// Returns a pointer to a new list Node with the relevant data attached
list_node_t *list_new_node(void *data, size_t dataSize);

/*
 * Converts the given list into an array
 */
array_t list_to_array(list_t list);

/*
 * Appends all given items to the list
 * NULL must be last argument
 */
list_t *list_append_multi_n(list_t *list, void *toAppend, ...);

/*
 * Returns a new list where the stored elements will be of size (elementSize)
 */
list_t new_list(size_t elementSize);

/*
 * Adds the data to the end of the list
 * Requires a pointer to the element that is going to be added (element data
 * gets copied in)
 */
list_t *list_append(list_t *list, void *data);

/*
 * Finds and returns the node at the given index
 * If the index is invalid, exits with code (-1)
 */
void *list_remove_at(list_t *list, unsigned int index);

void *list_remove_first(list_t *list);
void *list_remove_last(list_t *list);

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
list_t *list_combine(list_t *base, list_t *extension);

/*
 * frees all dynamically allocated data in a list.
 * IMPORTANT: Send a 'delete_data' function IF your data cannot be freed just by
 * calling free() on it's pointer
 */
void list_destroy(list_t *list, void (*delete_data)(void *data));

#endif