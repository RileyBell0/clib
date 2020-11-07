/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
*/

/*
 * This tree currently has no balancing
 * 
 * Collisions are handled through the use of a linked list at
 * the node at which the collision occurs. 
*/

/*
// TODO
maybe like get rid of collision support in the tree
*/

#ifndef CLIB_STD_TREE_H
#define CLIB_STD_TREE_H

#include "general.h"
#include "array.h"
#include "int.h"

#define RIGHT 1
#define LEFT -1

// Stores a reference to a single node in a tree
typedef struct avl_bin_tree_node_t
{
    void* data;
    struct avl_bin_tree_node_t *left;
    struct avl_bin_tree_node_t *right;
    int height;
} avl_bin_tree_node_t;

// Stores a reference to a single tree
// Every tree requires a compare function, this cannot be null
typedef struct avl_bin_tree_t
{
    avl_bin_tree_node_t *root;
    unsigned int elementSize;
    unsigned int size;
    int (*compare)(const void *first, const void *second);
} avl_bin_tree_t;

// returns a new tree struct with its values initialized to 0
avl_bin_tree_t new_avl_bin_tree(unsigned int elementSize, int (*compareFunc)(const void *first, const void *second));

// Inserts the given data into the tree using the search function
// stored in the tree
void avl_bin_tree_insert(avl_bin_tree_t *tree, void *data);

// destroys a tree, calling the delete_data function on every piece of stored data
void avl_bin_tree_destroy(avl_bin_tree_t tree, void (*delete_data)(void *data));

/* Given a binary tree, print its nodes in inorder*/
void avl_bin_tree_fill_arr(array_t *arr, avl_bin_tree_node_t* node, int *pos, unsigned int elementSize);

int avl_bin_tree_node_height(struct avl_bin_tree_node_t *N);

array_t avl_bin_tree_to_array(avl_bin_tree_t tree);

#endif