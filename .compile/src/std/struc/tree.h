#ifndef CLIB_STD_STRUC_TREE_H
#define CLIB_STD_STRUC_TREE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h> // TODO remove
#include <string.h>
#include "../memory.h"
#define TREE_NODE_ELEMENT 1

typedef struct tree_node_t
{
  struct tree_node_t *left;
  struct tree_node_t *right;
} tree_node_t;

typedef struct tree_t
{
  size_t elem_size;
  size_t size;
  bool allow_collisions;
  bool destroy_on_remove;
  tree_node_t *head;
  int (*compare)(const void *first, const void *second);
  void (*destroy)(void *data);
} tree_t;

tree_t tree_new(size_t elem_size,
                int (*compare)(const void *first, const void *second),
                void (*destroy)(void *data));

void tree_insert(tree_t *tree, void *data);

bool tree_remove(tree_t *tree, void *data);

void *tree_get(tree_t *tree, void *key);

bool tree_contains(tree_t *tree, void *key);

void *tree_get_min(tree_t *tree);

void *tree_get_max(tree_t *tree);

void *tree_pop_min(tree_t *tree);

void *tree_pop_max(tree_t *tree);

void tree_destroy(tree_t *tree);

#endif