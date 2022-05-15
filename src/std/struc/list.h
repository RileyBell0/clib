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

#ifndef CLIB_STD_STRUC_LIST_H
#define CLIB_STD_STRUC_LIST_H

#include "array.h"
#include "../sys/error.h"
#include "../index.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define ERROR -1
#define LIST_ELEMENT 1
#define NO_INDEX -1

/*
 * Struct for storing a single noede
 */
typedef struct list_node_t {
  struct list_node_t *next;
  struct list_node_t *prev;
} list_node_t;

/*
 * Struct for storing a single list
 */
typedef struct list_t {
  list_node_t *first_node;
  list_node_t *last_node;
  int size;
  size_t element_size;
  void (*delete_data)(void *data);
  int (*compare)(const void *first, const void *second);
} list_t;

typedef struct list_iterator_t {
  list_t *list;
  bool from_start;
  int index; // current node's index in the list
  list_node_t *curr_node;
  void *element;
  void *next_node;
  void *(*next)(struct list_iterator_t *iterator);
  bool (*done)(struct list_iterator_t *iterator);
  void *(*first)(struct list_iterator_t *iterator);
} list_iterator_t;

//////////////////////////////
// Initialisation
//////////////////////////////

/*
 * Returns a new list where the stored elements will be of size (elementSize)
 */
list_t list_new(size_t element_size);

list_iterator_t list_iterator_new(list_t *list, bool from_start);

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
 * Appends all given items to the list
 * NULL must be last argument
 */
list_t *list_append_multi(list_t *list, void *toAppend, ...);

/*
 * Removes the element at the given index, returning a pointer to it in
 * newly allocated memory -> note that this memory must be destroyed
 * If the index is invalid, exits with code EXIT_FAILURE
 *
 * Supports negative indicies
 */
void list_pop(list_t *list, void* dest, int index);

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
bool list_remove(list_t *list, void *elem);

// TODO implement, write help info
void list_clear(list_t *list);

//////////////////////////////
// High-level functions
//////////////////////////////

/*
 * Combines the two given lists, appending the list 'extension'
 * to the end of the list 'base'
 */
list_t *list_combine(list_t *base, list_t *extension);

/*
 * Converts the given list into an array
 */
array_t list_to_array(list_t *list);

//////////////////////////////
// Cleanup
//////////////////////////////

/*
 * Free all dynamically allocated memory in a list.
 * Variable delete_data should be a pointer to a function responsible for
 * freeing any heap allocated memory within each an element
 */
void list_destroy(list_t *list);

#endif