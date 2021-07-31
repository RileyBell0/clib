#ifndef CLIB_STD_AVL_TREE_H
#define CLIB_STD_AVL_TREE_H

#include "error.h"
#include "memory.h"
#include "array.h"
#include "int.h"
#include "dynamic_array.h"
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define AVL_TREE_NULL INT_MIN
#define AVL_TREE_ELEM 1

typedef struct avl_tree_node_t {
  int left;
  int right;
  int height;
} avl_tree_node_t;

typedef struct avl_tree_t {
  void *tree_start;
  int root;
  int size;
  int capacity;
  size_t elem_size;
  size_t block_size;
  bool destroy_on_remove;
  int (*compare)(const void *elem1, const void *elem2);
  void (*destroy)(void *data);
} avl_tree_t;

//////////////////////////////
// Initialisation
//////////////////////////////

/*
 * Returns a new, empty avl_tree. If NULL is recieved for the compare method,
 * will compare memory regions directly using memcmp.
 * destroy_on_remove is set to true by default
 */
avl_tree_t avl_tree_new(size_t elem_size,
                        int (*compare)(const void *elem1, const void *elem2));

//////////////////////////////
// Basic Operations
//////////////////////////////

/*
 * Inserts the given 'data' element into the avl tree
 */
void avl_tree_insert(avl_tree_t *tree, void *data);

#endif