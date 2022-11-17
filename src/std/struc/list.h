#ifndef CLIB_STD_STRUC_LIST_H
#define CLIB_STD_STRUC_LIST_H

#include "./array.h"
#include "../sys/error.h"
#include <stdbool.h>

#define ERROR -1
#define NO_INDEX -1

/*
 * A list node. Stores the element directly after the node's data
 * Memory allocated like this -> [sizeof(list_node_t), elem_size]
 */
typedef struct list_node_t
{
  struct list_node_t *next;
  struct list_node_t *prev;
} list_node_t;

/*
 * Struct for storing a single list
 */
typedef struct list_t
{
  struct list_node_t *first_node;
  struct list_node_t *last_node;
  size_t element_size;
  int size;
  void (*destroy)(void *data);
} list_t;

typedef struct list_iterator_t
{
  bool from_start;
  void *elem;
  struct list_node_t *node;
  int index; // current node's index in the list
  void *(*next)(struct list_iterator_t *iterator);
  bool (*done)(struct list_iterator_t *iterator);
} list_iterator_t;

/*
 * Returns a new list where the stored elements will be of size (elementSize)
 */
list_t list_new(size_t element_size, void (*destroy)(void *data));

/*
 * Creates a new list iterator
 * Usage: for (void *elem = it.elem; !it.done(&it); elem = it.next(&it))
 */
list_iterator_t list_iterator_new(list_t list, bool from_start);

//////////////////////////////
// Basic Operations
//////////////////////////////

/*
 * Given an index 'index', returns the element at the specified index
 * or exits with code EXIT_FAILURE on an invalid index
 * If the index is invalid, exits with code EXIT_FAILURE
 *
 * Supports negative indexing
 */
void *list_get(list_t *list, int index);

/*
 * Adds the data to the end of the list
 * Requires a pointer to the element that is going to be added (element data
 * gets copied in)
 */
list_t *list_append(list_t *list, void *data);

/*
 * Removes the element at the given index, returning a pointer to it in
 * newly allocated memory -> note that this memory must be destroyed
 * If the index is invalid, exits with code EXIT_FAILURE
 *
 * Supports negative indicies
 */
void list_pop(list_t *list, void *dest, int index);

/*
 * Removes the element at the given index from the list.
 * If the index is invalid, exits with code EXIT_FAILURE
 *
 * Supports negative indicies
 */
void list_remove_at(list_t *list, int index);

/*
 * Compares the bytes of length list->element_size between 'to_remove' and
 * each element. Removes the first element that is a match
 *
 * Returns true if an element was removed, false otherwise
 */
bool list_remove(list_t *list, void *key, int (*compare)(void *elem1, void *elem2));

//////////////////////////////
// High-level functions
//////////////////////////////

/*
 * Combines the two given lists, appending the list 'extension'
 * to the end of the list 'base'
 */
list_t *list_combine(list_t *base, list_t *extension);

/*
 * Free all dynamically allocated memory in a list.
 * delete_data should be a pointer to a function responsible for
 * freeing any heap allocated memory within each an element
 */
void list_destroy(list_t *list);

#endif