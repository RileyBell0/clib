#ifndef CLIB_STD_BLIST_H
#define CLIB_STD_BLIST_H

#include "array.h"
#include <stdarg.h>
#include <stdint.h>

#define BLIST_REALLOC_MULTIPLIER 2 // TODO currently unused
#define BLIST_NULL INT32_MIN
#define BLIST_SAFE_DESTROY
#define BLIST_ELEMENT 1
#define CLIB_STD_BLIST_COMPLEMENT TRUE

/*
 * All of these lists use -1 as a NULL value
 * but might consider changing this later
 */

// Using int32 for all values to demonstrate the capacity of this
// list being UINT32_T

typedef struct blist_node_t {
  int32_t next;
  int32_t prev;
} blist_node_t;

/*
 * Array based implementation of a list
 * Uses more space than an array or list, but is faster than both for
 * adding and removing elements. Max capacity of INT32_MAX
 */
typedef struct blist_t {
  void *list_start;
  int32_t *ledger; // node x is at index ledger[x] in the array
#ifdef CLIB_STD_BLIST_COMPLEMENT
  int32_t *ledger_complement; // ledger_complement[x] is node x in the list
#endif
  char destroy_on_remove;
  char fast_index;
  int32_t first;
  int32_t last;
  int32_t size;
  int32_t capacity;
  size_t element_size;
  size_t block_size;
  int (*compare)(const void *first, const void *second);
  void (*destroy)(void *data);
} blist_t;

#ifdef CLIB_STD_BLIST_COMPLEMENT
/*
 * Generates a complementary ledger for a given list
*/
int32_t* blist_generate_complementary_ledger(blist_t* list);
#endif

/*
 * returns the element at the given index, or Null
 * if out of bounds of the list
*/
void* blist_index(blist_t* list, int32_t index);

/*
 * Given a list, generates a ledger array for the blist
*/
int32_t* blist_generate_ledger(blist_t* list);

/*
 * Given two blists, combines them into list1 by appending list2
 * onto list1. returns list 1
*/
blist_t* blist_combine(blist_t* list1, blist_t *list2);

/*
 * Returns an empty blist - no memory allocated
 */
blist_t new_blist(size_t element_size);

/*
 * Appends the given item to the list, extending the list if neccessary
 */
void blist_append(blist_t *list, void *data);

/*
 * Gets the element in the blist at the given position
 */
void *blist_get_element(blist_t *list, uint32_t element);

/*
 * Given a node in an blist, removes it
 */
void blist_remove_node(blist_t *list, blist_node_t *node, uint32_t curr);

/*
 * Removes the first element in the blist matching the provided element
 */
int blist_remove(blist_t *list, void *element);

/*
 * Fixes the ledger for faster indexing 
 * Call immediately after removing a node
*/
void blist_fix_ledger(blist_t *list, int32_t removed_node);

/*
 * Ensures the given list has at least the capacity 'new_len', if not
 * resizes the list to fit
*/
void blist_set_length(blist_t *list, int32_t new_len);

/*
 * Given an blist, destroys its contents
*/
void blist_destroy(blist_t* list);

#endif
