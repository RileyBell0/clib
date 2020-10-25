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

#ifndef CLIB_STD_TREE_H
#define CLIB_STD_TREE_H

#include "general.h"
#include "list.h"

#define RIGHT 1
#define LEFT -1

// Stores a reference to a single node in a tree
typedef struct binary_tree_node_t
{
    list_t list;
    struct binary_tree_node_t *left;
    struct binary_tree_node_t *right;
} binary_tree_node_t;

// Stores a reference to a single tree
// Every tree requires a compare function, this cannot be null
typedef struct binary_tree_t
{
    binary_tree_node_t *root;
    unsigned int elementSize;
    unsigned int size;
    int (*compare)(void *first, void *second);
} binary_tree_t;

// returns a new tree struct with its values initialized to 0
binary_tree_t new_binary_tree(unsigned int elementSize, int (*compareFunc)(void *first, void *second));

// Inserts the given data into the tree using the search function
// stored in the tree
void binary_tree_insert(binary_tree_t *tree, void *data);

// destroys a tree, calling the delete_data function on every piece of stored data
void binary_tree_destroy(binary_tree_t tree, void (*delete_data)(void *data));

#endif