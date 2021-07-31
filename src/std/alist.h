#ifndef CLIB_STD_ALIST_H
#define CLIB_STD_ALIST_H

#include "array.h"
#include "string.h"
#include "index.h"
#include "pointer.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h> // TODO is this even used? i think limits.h provides all functionality
#include <stdio.h>
#include <limits.h>

#define ALIST_REALLOC_MULTIPLIER 2 // TODO currently unused
#define ALIST_NULL INT_MIN
#define ALIST_SAFE_DESTROY
#define ALIST_ELEMENT 1

/*
 * All of these lists use -1 as a NULL value
 * but might consider changing this later
 */


//////////////////////////////
// TODO validate the structures
// Structures
//////////////////////////////

/*
 * Records the relative positions of the next and previous nodes in the array
 * eg if this is node 3 and it is at position 2 in the array, and node 4 is at
 * position 1 in the array, the int 'next' will be -1
 */
typedef struct alist_node_t {
  int next;
  int prev;
} alist_node_t;

/*
 * Array based implementation of a list
 * Uses more space than an array or list, but is faster than both for
 * adding and removing elements. Max capacity of INT32_MAX
 */
typedef struct alist_t {
  void *list_start;
  bool destroy_on_remove;
  int first;
  int last;
  int size;
  int capacity;
  size_t element_size;
  size_t block_size;
  int (*compare)(const void *first, const void *second);
  void (*destroy)(void *data);
} alist_t;

typedef struct alist_iterator_t {
  alist_t *list;
  bool from_start;
  int index; // current node's index in the list
  int curr_node_pos;
  alist_node_t *curr_node;
  void *element;
  void *(*next)(struct alist_iterator_t *iterator);
  bool (*done)(struct alist_iterator_t *iterator);
  void *(*first)(struct alist_iterator_t *iterator);
} alist_iterator_t;

//////////////////////////////
// Initialization
//////////////////////////////

// Creates a list iterator for the alist
alist_iterator_t alist_iterator_new(alist_t *list, bool from_start);

/*
 * Returns an empty alist - no memory allocated
 */
alist_t alist_new(size_t element_size);

//////////////////////////////
// Basic Operations
//////////////////////////////

/*
 * Given an index 'index', returns a copy of the element at the specified
 * index.
 * - Exits with code EXIT_FAILURE if index is invalid
 * - Supports negative indexing
 * - Memory is allocated for return value
*/
void* alist_copy_element(alist_t *list, int index);

/* TODO make it actually exit
 * Given an index 'index', returns the element at the specified index
 * or exits with code EXIT_FAILURE on an invalid index
 * If the index is invalid, exits with code EXIT_FAILURE
 *
 * Supports negative indexing
 * 
 * The memory returned may be invalidated if an element is removed from
 * the list at any point after calling alist_get
 */
void *alist_get(alist_t *list, int index);

/*
 * Appends the given item to the alist
 * Requires a pointer to the element that is going to be added (element data
 * gets copied in)
 */
void alist_append(alist_t *list, void *data);

/*
 * Appends all given items to the alist
 * NULL must be last argument
 */
void alist_append_multi(alist_t *list, void *data, ...);

/*
 * Removes the element at the given index, returning a pointer to it in
 * newly allocated memory -> note that this memory must be destroyed
 * If the index is invalid, exits with code EXIT_FAILURE
 *
 * Supports negative indicies
 */
void *alist_pop(alist_t *list, int index);

/*
 * Removes the element at the given index from the list.
 * If the index is invalid, exits with code EXIT_FAILURE
 *
 * Supports negative indicies
 */
void alist_remove_at(alist_t *list, int index);

/*
 * Removes the first element in the alist matching the provided element
 */
bool alist_remove(alist_t *list, void *element);

// TODO implement
void alist_clear(alist_t *list);

//////////////////////////////
// High-level functions
//////////////////////////////

/*
 * Given two alists, combines them into list1 by appending list2
 * onto list1. returns list 1
 */
alist_t *alist_combine(alist_t *list1, alist_t *list2);

/*
 * Converts an alist into an array and returns it
 */
array_t alist_to_array(alist_t *list);

alist_t alist_copy(alist_t *list);

//////////////////////////////
// Utility
//////////////////////////////

// TODO write function comment
alist_node_t *alist_get_prev_node(alist_t *list, alist_node_t *node);
alist_node_t *alist_get_next_node(alist_t *list, alist_node_t *node);

/*
 * Ensures the given list has at least the capacity 'new_len', if not
 * resizes the list to fit
 */
void alist_set_length(alist_t *list, int new_len);

//////////////////////////////
// Cleanup
//////////////////////////////

/*
 * Given an alist, destroys its contents
 */
void alist_destroy(alist_t *list);

// TODO make it so that if you dont have a compare method it compares the bytes
// in the element, otherwise conmpares the elements based on the compare method
#endif
