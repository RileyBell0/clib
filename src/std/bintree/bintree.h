/*
 * This tree currently has no balancing
 * 
 * Collisions are handled through the use of a linked list at
 * the node at which the collision occurs. 
*/

#ifndef CLIB_TREE_H
#define CLIB_TREE_H

#include "../general/general.h"
#include "../list/list.h"

#define RIGHT 1
#define LEFT -1

// Stores a reference to a single node in a tree
typedef struct tree_node {
    list list;
    struct tree_node* left;
    struct tree_node* right;
}tree_node;


// Stores a reference to a single tree
typedef struct tree {
    tree_node* root;
}tree;

// returns a new tree struct with its values initialized to 0
tree new_tree();

void tree_insert(tree tree, void *data, int (*compare)(void *first, void *second));

// destroys a tree, calling the delete_data function on every piece of stored data
void tree_destroy(tree tree, void (*delete_data)(void* data));

#endif