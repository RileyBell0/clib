#ifndef CLIB_STD_ALIST_H
#define CLIB_STD_ALIST_H

#include "array.h"
#include "string.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define ALIST_REALLOC_MULTIPLIER 2 // TODO currently unused
#define ALIST_NULL INT32_MIN
#define ALIST_SAFE_DESTROY
#define ALIST_ELEMENT 1

/*
 * All of these lists use -1 as a NULL value
 * but might consider changing this later
 */

// Using int32 for all values to demonstrate the capacity of this
// list being unsigned int

typedef struct alist_node_t {
  unsigned int next;
  unsigned int prev;
} alist_node_t;

/*
 * Array based implementation of a list
 * Uses more space than an array or list, but is faster than both for
 * adding and removing elements. Max capacity of INT32_MAX
 */
typedef struct alist_t {
  void *list_start;
  bool destroy_on_remove;
  unsigned int first;
  unsigned int last;
  unsigned int size;
  unsigned int capacity;
  size_t element_size;
  size_t block_size;
  int (*compare)(const void *first, const void *second);
  void (*destroy)(void *data);
} alist_t;

typedef struct alist_iterator_t {
  alist_t *list;
  bool from_start;
  int next_node_pos;  // Next node's position in the array
  int curr_node_pos;  // Current node's position in the array
  unsigned int index; // current node's index in the list
  alist_node_t *curr_node;
  void *element;
  void *(*next)(struct alist_iterator_t *iterator);
  bool (*done)(struct alist_iterator_t *iterator);
  void *(*first)(struct alist_iterator_t *iterator);
} alist_iterator_t;

// Creates a list iterator for the alist
alist_iterator_t new_alist_iterator(alist_t *list, bool from_start);

/*
 * Converts an alist into an array and returns it
 */
array_t alist_to_array(alist_t *list);

/*
 * returns the element at the given index, or Null
 * if out of bounds of the list
 */
void *alist_index(alist_t *list, int index);

/*
 * Given two alists, combines them into list1 by appending list2
 * onto list1. returns list 1
 */
alist_t *alist_combine(alist_t *list1, alist_t *list2);

/*
 * Returns an empty alist - no memory allocated
 */
alist_t new_alist(size_t element_size);

/*
 * Appends the given item to the list, extending the list if neccessary
 */
void alist_append(alist_t *list, void *data);

/*
 * Gets the element in the alist at the given position
 */
void *alist_get_element(alist_t *list, unsigned int element);

/*
 * Given a node in an alist, removes it
 */
void alist_remove_node(alist_t *list, alist_node_t *node, unsigned int curr);

/*
 * Removes the first element in the alist matching the provided element
 */
bool alist_remove(alist_t *list, void *element);

/*
 * Ensures the given list has at least the capacity 'new_len', if not
 * resizes the list to fit
 */
void alist_set_length(alist_t *list, int new_len);

/*
 * Given an alist, destroys its contents
 */
void alist_destroy(alist_t *list);

#endif
