#ifndef CLIB_STD_ALIST_H
#define CLIB_STD_ALIST_H

#include "array.h"
#include <stdarg.h>
#include <stdint.h>

#define ALIST_REALLOC_MULTIPLIER 2 // TODO currently unused
#define ALIST_NULL INT32_MIN
#define ALIST_SAFE_DESTROY
#define ALIST_ELEMENT 1
// #define CLIB_STD_ALIST_COMPLEMENT TRUE

/*
 * All of these lists use -1 as a NULL value
 * but might consider changing this later
 */

// Using int32 for all values to demonstrate the capacity of this
// list being UINT32_T

typedef struct alist_node_t {
  int32_t next;
  int32_t prev;
} alist_node_t;

/*
 * Array based implementation of a list
 * Uses more space than an array or list, but is faster than both for
 * adding and removing elements. Max capacity of INT32_MAX
 */
typedef struct alist_t {
  void *list_start;
  int32_t *ledger; // node x is at index ledger[x] in the array
#ifdef CLIB_STD_ALIST_COMPLEMENT
  int32_t *ledger_complement; // ledger_complement[x] is node x in the list
#endif
  char fast_index;
  int32_t first;
  int32_t last;
  int32_t size;
  int32_t capacity;
  size_t element_size;
  size_t block_size;
  int (*compare)(const void *first, const void *second);
  void (*destroy)(void *data);
} alist_t;

#ifdef CLIB_STD_ALIST_COMPLEMENTARY
/*
 * Generates a complementary ledger for a given list
*/
int32_t* alist_generate_complementary_ledger(alist_t* list);
#endif


/*
 * Given a list, generates a ledger array for the alist
*/
int32_t* alist_generate_ledger(alist_t* list);

/*
 * Given two alists, combines them into list1 by appending list2
 * onto list1. returns list 1
*/
alist_t* alist_combine(alist_t* list1, alist_t *list2);

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
void *alist_get_element(alist_t *list, uint32_t element);

/*
 * Given a node in an alist, removes it
 */
void alist_remove_node(alist_t *list, alist_node_t *node, uint32_t curr);

/*
 * Removes the first element in the alist matching the provided element
 */
int alist_remove(alist_t *list, void *element);

/*
 * Fixes the ledger for faster indexing 
 * Call immediately after removing a node
*/
void alist_fix_ledger(alist_t *list, int32_t removed_node);

/*
 * Ensures the given list has at least the capacity 'new_len', if not
 * resizes the list to fit
*/
void alist_set_length(alist_t *list, int32_t new_len);

/*
 * Given an alist, destroys its contents
*/
void alist_destroy(alist_t* list);

#endif
